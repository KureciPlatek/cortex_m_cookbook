/**
 * Simple comm on UART with a flaw
 * Aim: test of TrustZone
 */
#ifndef __COMM_WITH_FLAW__
#define __COMM_WITH_FLAW__

#include "board.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/dma.h"

#define UART_ID      uart1
#define UART_TX_PIN  4
#define UART_RX_PIN  5
#define BAUD_RATE    921600
#define DATA_BITS    8
#define STOP_BITS    1
#define PARITY       UART_PARITY_NONE

// #define MAX_MSG_SIZE 32
typedef enum {
   READ_CMD_RPLY_CMD    = 0x00,
   READ_CMD_RPLY_DATA_0,
   READ_CMD_RPLY_DATA_1,
   READ_CMD_RPLY_DATA_2,
   READ_CMD_RPLY_DATA_3,
   READ_CMD_RPLY_ENDL,
   /* Keep at the end */
   READ_CMD_RPLY_SIZE
} READ_CMD_RPLY;

#define COMM_CMD_READ_SIZE    READ_CMD_RPLY_SIZE
#define CHANNEL_BUFFER_SIZE   64*COMM_CMD_READ_SIZE

#define DMA_IRQ_PRIORITY PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY
#define DMA_IRQ_RX_TO_USE 0
#define DMA_IRQ_TX_TO_USE 1


typedef enum {
   CHANN_STATE_NEW_DATA,
   CHANN_STATE_IDLE
} CHANN_STATE;

typedef struct {
   uint8_t buffer[CHANNEL_BUFFER_SIZE];
   int chann_nbr;
   CHANN_STATE state;
   dma_channel_config * handler;
} comm_channel;

typedef enum{
   COMM_CMD_READ       = 0x00,
   COMM_CMD_READ_RPLY  = 0x01,
   COMM_CMD_WRITE      = 0x02,
   COMM_CMD_WRITE_RPLY = 0x03,
   /* Keep at the end */
   COMM_CMD_MAX
} COMM_CMD;

typedef enum {
   COMM_MSG_STRUCT_CMD = 0,
   COMM_MSG_STRUCT_DATA,
   /* Keep at the end */
   COMM_MSG_STRUCT_MAX
} COMM_MSG_STRUCT;

typedef enum {
   REG_ID_TEMP = 0x01,
   REG_ID_VALA,
   REG_ID_VALB,
   /* Keep at the end */
   REG_ID_MAX
} REG_ID;

typedef struct
{
   uint32_t id;
   uint32_t * ptr;
} reg_t;

/**
 * @brief Process an incoming message
 * @param msg
 * @param len 
 * @return true if could be processed
 */
bool comm_process_msg(void);
bool comm_init(void);

#endif /* __COMM_WITH_FLAW__ */
