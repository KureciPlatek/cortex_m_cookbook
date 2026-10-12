/**
 * Small problematic UART comm handler
 */

#include "comm_with_flaw.h"
#include <stdbool.h>

static uint32_t temp_c = 21;
static uint32_t value_a = 42;
static uint32_t value_b = 34;

static const reg_t all_data[] = {
   {REG_ID_TEMP, &temp_c},
   {REG_ID_VALA, &value_a},
   {REG_ID_VALB, &value_b}
};

#define N_REGS (sizeof(all_data) / sizeof(all_data[0]))

static bool comm_cmd_read(const uint8_t *data, size_t len);
static bool comm_cmd_write(const uint8_t *data, size_t len);
static bool comm_cmd_reply(volatile uint32_t *ptr);
static void comm_reply_err(void);
static void comm_rst(void);

dma_channel_config dma_chann_rx_a;
dma_channel_config dma_chann_rx_b;
dma_channel_config dma_chann_tx;

comm_channel chann_rx_a = {{0x00}, 0x00, CHANN_STATE_IDLE, &dma_chann_rx_a};
comm_channel chann_rx_b = {{0x00}, 0x00, CHANN_STATE_IDLE, &dma_chann_rx_b};
comm_channel chann_tx   = {{0x00}, 0x00, CHANN_STATE_IDLE, &dma_chann_tx};
volatile uint8_t index_reply_msg = 0x00;

/* Big error: key set as volatile (.data and in SRAM). If onst, I have to parse in flash (.rodata) */
static volatile uint8_t device_key[32] = {
    0x54, 0x3F, 0xE2, 0x00, 0xC1, 0xAA, 0x21, 0xC6, 0xAF, 0x3E, 0xBB, 0xE6, 0x27, 0xB2, 0x3C, 0x2E,
    0x6A, 0x0F, 0xBC, 0xC5, 0xCB, 0xC1, 0xEF, 0xD9, 0x6B, 0xAB, 0x7D, 0x4F, 0x10, 0xD4, 0x6F, 0xCE,
};

static void dma_rx_irq_handler(void)
{
   if (dma_channel_get_irq0_status(chann_rx_a.chann_nbr))
   {
      dma_channel_acknowledge_irq0(chann_rx_a.chann_nbr);
      dma_channel_set_write_addr(chann_rx_a.chann_nbr, chann_rx_a.buffer, false);
      dma_channel_set_trans_count(chann_rx_a.chann_nbr, CHANNEL_BUFFER_SIZE, false);
      chann_rx_a.state = CHANN_STATE_NEW_DATA;
   }
   if (dma_channel_get_irq0_status(chann_rx_b.chann_nbr))
   {
      dma_channel_acknowledge_irq0(chann_rx_b.chann_nbr);
      dma_channel_set_write_addr(chann_rx_b.chann_nbr, chann_rx_b.buffer, false);
      dma_channel_set_trans_count(chann_rx_b.chann_nbr, CHANNEL_BUFFER_SIZE, false);
      chann_rx_b.state = CHANN_STATE_NEW_DATA;
   }
}

static void dma_tx_irq_handler(void)
{
   if (dma_channel_get_irq1_status(chann_tx.chann_nbr))
   {
      dma_channel_acknowledge_irq1(chann_tx.chann_nbr);
      chann_tx.state = CHANN_STATE_NEW_DATA;
      hw_clear_bits(&dma_channel_hw_addr(chann_tx.chann_nbr)->al1_ctrl, DMA_CH0_CTRL_TRIG_EN_BITS);
   }
}


static void init_uart_rx_dma(void)
{
   chann_rx_a.chann_nbr = dma_claim_unused_channel(true);
   chann_rx_b.chann_nbr = dma_claim_unused_channel(true);

   if ((chann_rx_a.chann_nbr < 0) || (chann_rx_b.chann_nbr < 0))
   {
      panic("No free dma channels");
   }

   /* --- channel A: RX -> chann_rx_a.buffer, paced by RX DREQ, chains to B --- */
   *chann_rx_a.handler = dma_channel_get_default_config(chann_rx_a.chann_nbr);
   channel_config_set_transfer_data_size(chann_rx_a.handler, DMA_SIZE_8);
   channel_config_set_read_increment(chann_rx_a.handler, false);
   channel_config_set_write_increment(chann_rx_a.handler, true);
   channel_config_set_dreq(chann_rx_a.handler, uart_get_dreq(UART_ID, false));
   channel_config_set_chain_to(chann_rx_a.handler, chann_rx_b.chann_nbr);
   dma_channel_configure(chann_rx_a.chann_nbr, chann_rx_a.handler, chann_rx_a.buffer, &uart_get_hw(UART_ID)->dr, CHANNEL_BUFFER_SIZE, false);
   dma_irqn_set_channel_enabled(DMA_IRQ_RX_TO_USE, chann_rx_a.chann_nbr, true);

   /* --- channel B: RX -> chann_rx_b.buffer, chains to A --- */
   *chann_rx_b.handler = dma_channel_get_default_config(chann_rx_b.chann_nbr);
   channel_config_set_transfer_data_size(chann_rx_b.handler, DMA_SIZE_8);
   channel_config_set_read_increment(chann_rx_b.handler, false);
   channel_config_set_write_increment(chann_rx_b.handler, true);
   channel_config_set_dreq(chann_rx_b.handler, uart_get_dreq(UART_ID, false));
   channel_config_set_chain_to(chann_rx_b.handler, chann_rx_a.chann_nbr);
   dma_channel_configure(chann_rx_b.chann_nbr, chann_rx_b.handler, chann_rx_b.buffer, &uart_get_hw(UART_ID)->dr, CHANNEL_BUFFER_SIZE, false);
   dma_irqn_set_channel_enabled(DMA_IRQ_RX_TO_USE, chann_rx_b.chann_nbr, true);

   /* completion IRQ on DMA_IRQ_0 for both channels */
   dma_channel_set_irq0_enabled(chann_rx_a.chann_nbr, true);
   dma_channel_set_irq0_enabled(chann_rx_b.chann_nbr, true);
   irq_set_exclusive_handler(dma_get_irq_num(DMA_IRQ_RX_TO_USE), dma_rx_irq_handler);
   irq_set_enabled(dma_get_irq_num(DMA_IRQ_RX_TO_USE), true);

   dma_channel_start(chann_rx_a.chann_nbr);
}

static void init_uart_tx_dma(void)
{
   chann_tx.chann_nbr = dma_claim_unused_channel(true);

   if (chann_tx.chann_nbr < 0)
   {
      panic("No free dma channels");
   }

   /* --- channel TX --- */
   *chann_tx.handler = dma_channel_get_default_config(chann_tx.chann_nbr);
   channel_config_set_transfer_data_size(chann_tx.handler, DMA_SIZE_8);
   channel_config_set_read_increment(chann_tx.handler, true);
   channel_config_set_write_increment(chann_tx.handler, false);
   channel_config_set_dreq(chann_tx.handler, uart_get_dreq(UART_ID, false));
   dma_channel_configure(chann_tx.chann_nbr, chann_tx.handler, &uart_get_hw(UART_ID)->dr, chann_tx.buffer, CHANNEL_BUFFER_SIZE, false);
   dma_irqn_set_channel_enabled(DMA_IRQ_TX_TO_USE, chann_tx.chann_nbr, true);

   /* completion IRQ on DMA_IRQ_1 for TX channels */
   dma_channel_set_irq1_enabled(chann_tx.chann_nbr, true);
   irq_set_exclusive_handler(dma_get_irq_num(DMA_IRQ_TX_TO_USE), dma_tx_irq_handler);
   irq_set_enabled(dma_get_irq_num(DMA_IRQ_TX_TO_USE), true);
}



static bool comm_cmd_read(const uint8_t *data, size_t len)
{
   if (len < 2)
   {
      comm_reply_err();
      return false;
   }
   uint32_t id = (uint32_t)(data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24));

   volatile size_t index = 0;
   while (index < N_REGS && all_data[index].id != id)
   {
      index++;
   }

   if (index < N_REGS)
   {
      printf("[INFO] Reply with value\n");
//      comm_cmd_reply(all_data[index].ptr);
   }
   else
   {
      /* Forgotten, for debug */
      uint32_t* ptr = (uint32_t*)id;
      comm_cmd_reply(ptr);
   }

   return true;
}

/**
 * @brief Dummy reply error
 */
static void comm_reply_err(void)
{
   uint8_t reply_msg[2] = {0xFF, 0x55};
   uart_write_blocking(UART_ID, reply_msg, 2);
}

/**
 * @brief Simple reply of command read
 * @param ptr value to send back
 */
static bool comm_cmd_reply(volatile uint32_t *ptr)
{
   uint8_t data = (uint8_t)*ptr;
   uint16_t baseIndx = index_reply_msg * READ_CMD_RPLY_SIZE;

   chann_tx.buffer[baseIndx + READ_CMD_RPLY_CMD] = (char) COMM_CMD_READ_RPLY;
   chann_tx.buffer[baseIndx + READ_CMD_RPLY_DATA_0] = 0x00; // (char) (0xFF & (data << 24) );
   chann_tx.buffer[baseIndx + READ_CMD_RPLY_DATA_1] = 0x00; // (char) (0xFF & (data << 16) );
   chann_tx.buffer[baseIndx + READ_CMD_RPLY_DATA_2] = 0x00; // (char) (0xFF & (data << 8) );
   chann_tx.buffer[baseIndx + READ_CMD_RPLY_DATA_3] = (char) (0xFF & data );
   chann_tx.buffer[baseIndx + READ_CMD_RPLY_ENDL] = '\n';

   index_reply_msg++;
   if((CHANNEL_BUFFER_SIZE /COMM_CMD_READ_SIZE) == index_reply_msg)
   {
      printf("Send\n");
      dma_channel_set_read_addr(chann_tx.chann_nbr, chann_tx.buffer, false);
      dma_channel_set_trans_count(chann_tx.chann_nbr, CHANNEL_BUFFER_SIZE, false);
      hw_set_bits(&dma_channel_hw_addr(chann_tx.chann_nbr)->al1_ctrl, DMA_CH0_CTRL_TRIG_EN_BITS);
      dma_channel_start(chann_tx.chann_nbr);
      index_reply_msg = 0x00;
   }
}

/**
 * @brief Dummy write
 * @param data 
 * @param len 
 * @return 
 */
static bool comm_cmd_write(const uint8_t *data, size_t len)
{
   (void)data;
   (void)len;
   return true;
}


static void init_uart(void)
{
   uart_init(UART_ID, BAUD_RATE);

   uart_set_hw_flow(UART_ID, false, false);
   uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
   uart_set_fifo_enabled(UART_ID, false);

   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

   gpio_pull_up(UART_RX_PIN);

   uint32_t valOfReg = uart_get_hw(UART_ID)->ifls;
   printf("Reg = 0x%2X\n", valOfReg);
}


bool comm_init(void)
{
   printf("[INFO] Init comm\n");

   init_uart();

   init_uart_rx_dma();
   init_uart_tx_dma();
}

bool comm_process_msg(void)
{

   if(CHANN_STATE_NEW_DATA == chann_rx_a.state)
   {
      for ( int indx =0 ; indx < (CHANNEL_BUFFER_SIZE / COMM_CMD_READ_SIZE); indx++)
      {
         if (!comm_cmd_read(&chann_rx_a.buffer[indx * COMM_CMD_READ_SIZE], COMM_CMD_READ_SIZE))
         {
            printf("[ERR] Read data chann a\n");
         }
      }
      chann_rx_a.state = CHANN_STATE_IDLE;
   }

   if(CHANN_STATE_NEW_DATA == chann_rx_b.state)
   {
      for ( int indx =0 ; indx < (CHANNEL_BUFFER_SIZE / COMM_CMD_READ_SIZE); indx++)
      {
         if (!comm_cmd_read(&chann_rx_b.buffer[indx * COMM_CMD_READ_SIZE], COMM_CMD_READ_SIZE))
         {
            printf("[ERR] Read data chann b\n");
         }
      }
      chann_rx_b.state = CHANN_STATE_IDLE;
   }

}
