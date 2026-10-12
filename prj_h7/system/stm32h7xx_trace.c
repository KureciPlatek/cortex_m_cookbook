/**
 * @file    stm32h7xx_trace.c
 * @author  Jeremie Gallee (jeremie.gallee@noser.com)
 * @brief   Manage CoreSight unsupported options by STM32_HAL.
 *          Allows to configure and use SWO, parallel trace, ITM, etc...
 * @version 0.1
 * @date    2026-07-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "stm32h7xx_trace.h"
#include "board.h"

/**
 * @brief Initiate the debug block of STM32. So TPIU
 */
void initDebugBlock(void)
{
   LL_DBGMCU_EnableTracePortClock(); /* For parallel trace */
   LL_DBGMCU_EnableD1DebugClock();
   LL_DBGMCU_EnableD3DebugClock();
   __DSB();
}

/**
  \brief   SWO Configure
  \param [in]  protocol   \ref SWO_SPPR_TXMODE_Manchester or
                          \ref SWO_SPPR_TXMODE_NRZ.
  \param [in]  SystemCoreClock Frequency in Hz of System clock core.
  \param [in]  baudrate   Requested output bit rate in Hz.
  \return             0  Configuration applied and verified.
  \return             1  Component absent
  \return             2  Baudrate or SystemCoreClock is 0
  \return             3  Error by calculated prescaler
  \return             4  Prescaler could not be written in SWO register
 */
uint32_t configureSWO (uint32_t protocol, uint32_t SystemCoreClock, uint32_t baudrate)
{
   uint32_t prescaler = 0x0000;

   if (SWO_CheckPresence() == 0UL)
   {
      return (1UL);
   }
   if ((baudrate == 0UL) || (SystemCoreClock == 0UL))
   {
      return (2UL);
   }

   prescaler = (SystemCoreClock / baudrate) - 1;

   if (prescaler > SWO_CODR_PRESCALER_Msk)
   {
      return (3UL);
   }

   SWO_Unlock();
   SWO_SetProtocol(protocol);
   SWO_SetPrescaler(prescaler);

   /* Activate channel 0 to 3 of ITM to transfer data over channel 2 (not only characters) */
   ITM->TER = 0x000F;
   CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
   DWT->LAR = CS_KEY; /* Write 0xC5ACCE55UL to DWT to modify it */
   DWT->CYCCNT = 0;
   /* Start counting */
   DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

   return ((SWO_GetPrescaler() == prescaler) ? 4UL : 0UL);
}

bool traceWaitBitSet(__IM uint32_t *reg, uint32_t mask)
{
   uint32_t spins = 100000U;
   while (READ_BIT(*reg, mask) == RESET) {
      spins--;
      if(spins <= 0U) { return false; }
   }
   return true;
}

bool traceWaitBitUnset(__IM uint32_t *reg, uint32_t mask)
{
   uint32_t spins = 100000U;
   while (READ_BIT(*reg, mask) != RESET) {
      spins--;
      if(spins <= 0U) { return false; }
   }
   return true;
}

/**
 * @brief Configure ETF. ETM goes through it so have to configure it
 * @param mode is HW_FIFO
 * @return true if could configure ETM
 *         false if not
 */
static bool ETM_ConfigureDefault(void)
{
   CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
   __DSB();

   ETM->LAR = CS_KEY;
   __DSB();

   ETM->PRGCTL = 0U; /* disable, let the pipeline drain    */
   __DSB();
   /* Wait for IDLE state */
   if (!traceWaitBitSet(&ETM->STAT, ETM_STAT_IDLE_Msk))
   {
      printf("ETM->STAT = 0x%lx\n", ETM->STAT);
      return false;
   }
   /* Wait for registers to be stable */
   if (!traceWaitBitSet(&ETM->STAT, ETM_STAT_PMSTABLE_Msk))
   {
      printf("ETM->STAT = 0x%lx\n", ETM->STAT);
      return false;
   }

   /* Configure tracing information (what is to be traced)
    * - ON: Timestamps + return stack
    * - OFF: branch-broadcast, conditional Instruction, cycle-count, events.
    * Keep at minimal, maybe later activate all, but increase data being outputed
    */
   ETM->CONFIG    = 0U;
   ETM->CONFIG   |= ETM_CONFIG_TS_Msk | ETM_CONFIG_RS_Msk;
   ETM->EVENTCTL0 = 0U;
   ETM->EVENTCTL1 = 0U;

   /* Stall the core rather than overflow the FIFO */
   ETM->STALLCTL = 0x0000;
//   ETM->STALLCTL = ETM_STALLCTL_ISTALL_Msk | ((2UL << ETM_STALLCTL_LEVEL_Pos) & ETM_STALLCTL_LEVEL_Msk);

   ETM->TSCTL = 0x00000000UL; /* timestamp insertion resource: none (no global timestamp) */
   /* SYNCP is read-only here (fixed 1024 bytes) -- not written. */
   ETM->TRACEID = TRACE_ID_ETM & ETM_TRACEID_TRACEID_Msk;

   /* ViewInst: trace everything. SEL = always-true resource, SSSTATUS started. */
   ETM->VICTL = (ETM_VICTL_RESOURCE_TRUE << ETM_VICTL_SEL_Pos) | ETM_VICTL_SSSTATUS_Msk;
   ETM->VISSCTL = 0U;

   __DSB();
   __ISB();
   return true;
}

static bool TPIU_ConfigureParallel(void)
{
   TPIU_H7->LAR = CS_KEY;
   __DSB();

   /* SSPSR bit n set => an (n+1)-bit port is supported by this implementation */
   if ((TPIU_H7->SUPPSIZE & (1UL << (TRACE_PORT_WIDTH - 1U))) == 0U)
   {
      return false;
   }

   /* Flush trace first and stop. Then do modifications */
   /* Have to check first if TPIU->CURTPM is deactivated, otherwise it sends data
    continuously and TRACE can't be stopped (it seems)
    The reason is that TPIU_H7 is in Debug domain and this domain is not reset when
    doing a new flash or soft reset cycle. Only a hard cycle would reset it */
   if(READ_BIT(TPIU_H7->CURTPM, TPIU_CURTPM_PCONTEN_Msk) > 0x00)
   {
      CLEAR_BIT(TPIU_H7->CURTPM, TPIU_CURTPM_PCONTEN_Msk);
   }
   TPIU_H7->FFCR = TPIU_FFCR_STOPFL_Msk | TPIU_FFCR_FONMAN_Msk;
   __DSB();
   if (!traceWaitBitSet(&TPIU_H7->FFSR, TPIU_FFSR_FTSTOPPED_Msk)) {
      return false;
   }

   TPIU_H7->CURPSIZE = 1UL << (TRACE_PORT_WIDTH - 1U);
   printf("0x%lx\n", TPIU_H7->SUPPSIZE);

   /* Frame sync period. Decoders need these to find frame boundaries; too
    * rare and one dropped byte costs a long resync. */
   TPIU_H7->FSCR = 0x40U & TPIU_FSCR_CYCCOUNT_Msk;

   /* Set an IDLE pattern - uncomment to have some default values on D0-3 lines and confirm TPIU/TRACE works */
//   TPIU_H7->CURTPM |= TPIU_CURTPM_PCONTEN_Msk | TPIU_CURTPM_PATA5_Msk; /* Set to AA55 pattern */

   /* Enable formatter */
   TPIU_H7->FFCR = TPIU_FFCR_ENFCONT_Msk | TPIU_FFCR_ENFTC_Msk;
   __DSB();
   return true;
}

static bool ETM_Enable(void)
{
   ETM->PRGCTL |= ETM_PRGCTL_EN_Msk;
   __DSB();
   __ISB();
   return traceWaitBitUnset(&ETM->STAT, ETM_STAT_IDLE_Msk);
}

static void CSTF_Configure(void)
{
   CSTF->LAR = 0xC5ACCE55UL; /* unlock */
   __DSB();

   /* Enable S0 (ETM) */
   CSTF->CTRL = (CSTF->CTRL & ~CSTF_CTRL_ENS1_Msk) | CSTF_CTRL_ENS0_Msk;
   __DSB();
}

void ETM_Disable(void)
{
   ETM->PRGCTL = 0U;
   __DSB();
   (void)traceWaitBitSet(&ETM->STAT, ETM_STAT_IDLE_Msk);
}

bool parallel_trace_start(void)
{
//  TRACE_ClocksInit(); /* Already done */
//  TRACE_PinsInit();   /* Already done */

//  if (!ETM_Present())             { return false; } /* Should be 0x4100f401 at @0xE00411E4 and it is */
  if (!TPIU_ConfigureParallel()) { return false; }
  if (!ETM_ConfigureDefault())    { return false; }
  CSTF_Configure();

  return ETM_Enable();
}
