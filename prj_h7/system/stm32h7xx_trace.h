/**
 * @file    stm32h7xx_trace.h
 * @author  Jeremie Gallee (jeremie.gallee@noser.com)
 * @brief   SWO tracing registers from Cortex M7 for STM32H723ZG
 *          STMicroelectronics doesn't provide the header for those registers as they will be configured
 *          with STLink. It could be actually possible to do so with other debuggers, (attack then 
 *          debugger base address at 0xE00E3000 instead of 0x5C003000)
 *          It is like an extension os CMSIS for SWO manipulation on STM32H723ZG
 * @version 0.1
 * @date    2026-07-10
 * 
 * @copyright Copyright (c) 2026
 * 
 * @remark  With a debugger, STM32H723 datasheet specifies that to access debug registers
 *          we have to access to "component base address", so 0xE00Exxxx, but with openOCD
 *          and gdb, not, r/w is done as if we access to system bus, so address 0x5C00xxxx
 *
 * @remark  For ETM, addresses are at @0xE0041000.
 *          Confirmed with ETM_IDR1 @0xE00411E4 = 0x4100f401 with gdb
 */

#ifndef STM32H7XX_TRACE_H
#define STM32H7XX_TRACE_H

#include "stm32h7xx.h"
#include <stdbool.h>

#ifdef __cplusplus
 extern "C" {
#endif
 
#ifndef __SWO_PRESENT
  #define __SWO_PRESENT             1U
#endif
 
#if defined (__SWO_PRESENT) && (__SWO_PRESENT == 1U)

/* ------------------------------------------------------------------------- */
/*    SWO                                                                    */
/* ------------------------------------------------------------------------- */

/**
  \brief  Structure type to access the Serial Wire Output Register (ITM).
 */
typedef struct
{
        uint32_t RESERVED0[4U];
  __IOM uint32_t CODR;                   /*!< Offset: 0x010 (R/W)  SWO current output divisor Register */
        uint32_t RESERVED1[55U];
  __IOM uint32_t SPPR;                   /*!< Offset: 0x0F0 (R/W)  SWO selected pin protocol Register */
        uint32_t RESERVED2[131U];
  __IM  uint32_t FFSR;                   /*!< Offset: 0x300 (R/ )  SWO formatter and flush status Register */
        uint32_t RESERVED3[807U];
  __IOM uint32_t CLAIMSET;               /*!< Offset: 0xFA0 (R/W)  SWO claim tag set Register */
  __IOM uint32_t CLAIMCLR;               /*!< Offset: 0xFA4 (R/W)  SWO claim tag clear Register */
        uint32_t RESERVED5[2U];
  __OM  uint32_t LAR;                    /*!< Offset: 0xFB0 ( /W)  SWO lock access Register */
  __IM  uint32_t LSR;                    /*!< Offset: 0xFB4 (R/ )  SWO lock status Register */
  __IM  uint32_t AUTHSTAT;               /*!< Offset: 0xFB8 (R/ )  SWO authentication status Register */
        uint32_t RESERVED6[3U];
  __IM  uint32_t DEVID;                  /*!< Offset: 0xFC8 (R/ )  SWO device configuration Register */
  __IM  uint32_t DEVTYPE;                /*!< Offset: 0xFCC (R/ )  SWO device type identifier Register */
  __IM  uint32_t PIDR4;                  /*!< Offset: 0xFD0 (R/ )  SWO CS Peripheral Identification Register #4 */
        uint32_t RESERVED7[3U];
  __IM  uint32_t PIDR0;                  /*!< Offset: 0xFE0 (R/ )  SWO CS Peripheral Identification Register #0 */
  __IM  uint32_t PIDR1;                  /*!< Offset: 0xFE4 (R/ )  SWO CS Peripheral Identification Register #1 */
  __IM  uint32_t PIDR2;                  /*!< Offset: 0xFE8 (R/ )  SWO CS Peripheral Identification Register #2 */
  __IM  uint32_t PIDR3;                  /*!< Offset: 0xFEC (R/ )  SWO CS Peripheral Identification Register #3 */
  __IM  uint32_t CIDR0;                  /*!< Offset: 0xFF0 (R/ )  SWO CS Component  Identification Register #0 */
  __IM  uint32_t CIDR1;                  /*!< Offset: 0xFF4 (R/ )  SWO CS Component  Identification Register #1 */
  __IM  uint32_t CIDR2;                  /*!< Offset: 0xFF8 (R/ )  SWO CS Component  Identification Register #2 */
  __IM  uint32_t CIDR3;                  /*!< Offset: 0xFFC (R/ )  SWO CS Component  Identification Register #3 */
} SWO_Type;



/* SWO Current Output Divisor Register Definitions */
#define SWO_CODR_PRESCALER_Pos              0U                                         /*!< SWO CODR: PRESCALER Position */
#define SWO_CODR_PRESCALER_Msk             (0x1FFFUL /*<< SWO_CODR_PRESCALER_Pos*/)    /*!< SWO CODR: PRESCALER Mask */
 
/* SWO Selected Pin Protocol Register Definitions */
#define SWO_SPPR_TXMODE_Pos                 0U                                         /*!< SWO SPPR: TXMODE Position */
#define SWO_SPPR_TXMODE_Msk                (0x3UL /*<< SWO_SPPR_TXMODE_Pos*/)          /*!< SWO SPPR: TXMODE Mask */
 
#define SWO_SPPR_TXMODE_Parallel           (0x0UL /*<< SWO_SPPR_TXMODE_Pos*/)          /*!< SWO SPPR: Parallel trace port (reserved) */
#define SWO_SPPR_TXMODE_Manchester         (0x1UL /*<< SWO_SPPR_TXMODE_Pos*/)          /*!< SWO SPPR: SWO Manchester encoding */
#define SWO_SPPR_TXMODE_NRZ                (0x2UL /*<< SWO_SPPR_TXMODE_Pos*/)          /*!< SWO SPPR: SWO NRZ (UART) encoding */
 
/* SWO Formatter and Flush Status Register Definitions */
#define SWO_FFSR_FtNonStop_Pos              3U                                         /*!< SWO FFSR: FtNonStop Position */
#define SWO_FFSR_FtNonStop_Msk             (0x1UL << SWO_FFSR_FtNonStop_Pos)           /*!< SWO FFSR: FtNonStop Mask */
 
#define SWO_FFSR_TCPresent_Pos              2U                                         /*!< SWO FFSR: TCPresent Position */
#define SWO_FFSR_TCPresent_Msk             (0x1UL << SWO_FFSR_TCPresent_Pos)           /*!< SWO FFSR: TCPresent Mask */
 
#define SWO_FFSR_FtStopped_Pos              1U                                         /*!< SWO FFSR: FtStopped Position */
#define SWO_FFSR_FtStopped_Msk             (0x1UL << SWO_FFSR_FtStopped_Pos)           /*!< SWO FFSR: FtStopped Mask */
 
#define SWO_FFSR_FlInProg_Pos               0U                                         /*!< SWO FFSR: FlInProg Position */
#define SWO_FFSR_FlInProg_Msk              (0x1UL /*<< SWO_FFSR_FlInProg_Pos*/)        /*!< SWO FFSR: FlInProg Mask */
 
/* SWO Claim Tag Set/Clear Register Definitions */
#define SWO_CLAIM_TAG_Pos                   0U                                         /*!< SWO CLAIM: TAG Position */
#define SWO_CLAIM_TAG_Msk                  (0xFUL /*<< SWO_CLAIM_TAG_Pos*/)            /*!< SWO CLAIM: TAG Mask */
 
/* SWO Lock Status Register Definitions */
#define SWO_LSR_nTT_Pos                     2U                                         /*!< SWO LSR: Not thirty-two bit Position */
#define SWO_LSR_nTT_Msk                    (0x1UL << SWO_LSR_nTT_Pos)                  /*!< SWO LSR: Not thirty-two bit Mask */
 
#define SWO_LSR_SLK_Pos                     1U                                         /*!< SWO LSR: Software Lock status Position */
#define SWO_LSR_SLK_Msk                    (0x1UL << SWO_LSR_SLK_Pos)                  /*!< SWO LSR: Software Lock status Mask */
 
#define SWO_LSR_SLI_Pos                     0U                                         /*!< SWO LSR: Software Lock implemented Position */
#define SWO_LSR_SLI_Msk                    (0x1UL /*<< SWO_LSR_SLI_Pos*/)              /*!< SWO LSR: Software Lock implemented Mask */
 
/* SWO Authentication Status Register Definitions */
#define SWO_AUTHSTAT_SNID_Pos               6U                                         /*!< SWO AUTHSTAT: SNID Position */
#define SWO_AUTHSTAT_SNID_Msk              (0x3UL << SWO_AUTHSTAT_SNID_Pos)            /*!< SWO AUTHSTAT: SNID Mask */
 
#define SWO_AUTHSTAT_SID_Pos                4U                                         /*!< SWO AUTHSTAT: SID Position */
#define SWO_AUTHSTAT_SID_Msk               (0x3UL << SWO_AUTHSTAT_SID_Pos)             /*!< SWO AUTHSTAT: SID Mask */
 
#define SWO_AUTHSTAT_NSNID_Pos              2U                                         /*!< SWO AUTHSTAT: NSNID Position */
#define SWO_AUTHSTAT_NSNID_Msk             (0x3UL << SWO_AUTHSTAT_NSNID_Pos)           /*!< SWO AUTHSTAT: NSNID Mask */
 
#define SWO_AUTHSTAT_NSID_Pos               0U                                         /*!< SWO AUTHSTAT: NSID Position */
#define SWO_AUTHSTAT_NSID_Msk              (0x3UL /*<< SWO_AUTHSTAT_NSID_Pos*/)        /*!< SWO AUTHSTAT: NSID Mask */
 
/* SWO Device Configuration Register Definitions                              */
/* NOTE: this is the TPIU-style DEVID encoding. The standalone SWO component  */
/*       may not populate every field identically - confirm against RM0468 /  */
/*       the CoreSight SWO TRM before relying on these bits.                  */
#define SWO_DEVID_NRZVALID_Pos             11U                                         /*!< SWO DEVID: NRZVALID Position */
#define SWO_DEVID_NRZVALID_Msk             (0x1UL << SWO_DEVID_NRZVALID_Pos)           /*!< SWO DEVID: NRZVALID Mask */
 
#define SWO_DEVID_MANCVALID_Pos            10U                                         /*!< SWO DEVID: MANCVALID Position */
#define SWO_DEVID_MANCVALID_Msk            (0x1UL << SWO_DEVID_MANCVALID_Pos)          /*!< SWO DEVID: MANCVALID Mask */
 
#define SWO_DEVID_PTINVALID_Pos             9U                                         /*!< SWO DEVID: PTINVALID Position */
#define SWO_DEVID_PTINVALID_Msk            (0x1UL << SWO_DEVID_PTINVALID_Pos)          /*!< SWO DEVID: PTINVALID Mask */
 
#define SWO_DEVID_FIFOSZ_Pos                6U                                         /*!< SWO DEVID: FIFOSZ Position */
#define SWO_DEVID_FIFOSZ_Msk               (0x7UL << SWO_DEVID_FIFOSZ_Pos)             /*!< SWO DEVID: FIFOSZ Mask */
 
/* SWO Device Type Identifier Register Definitions */
#define SWO_DEVTYPE_SubType_Pos             4U                                         /*!< SWO DEVTYPE: SubType Position */
#define SWO_DEVTYPE_SubType_Msk            (0xFUL << SWO_DEVTYPE_SubType_Pos)          /*!< SWO DEVTYPE: SubType Mask */
 
#define SWO_DEVTYPE_MajorType_Pos           0U                                         /*!< SWO DEVTYPE: MajorType Position */
#define SWO_DEVTYPE_MajorType_Msk          (0xFUL /*<< SWO_DEVTYPE_MajorType_Pos*/)    /*!< SWO DEVTYPE: MajorType Mask */
 
/* SWO Peripheral Identification Register Definitions */
#define SWO_PIDR4_SIZE_Pos                  4U                                         /*!< SWO PIDR4: 4KB count Position */
#define SWO_PIDR4_SIZE_Msk                 (0xFUL << SWO_PIDR4_SIZE_Pos)               /*!< SWO PIDR4: 4KB count Mask */
 
#define SWO_PIDR4_DES_2_Pos                 0U                                         /*!< SWO PIDR4: JEP106 continuation Position */
#define SWO_PIDR4_DES_2_Msk                (0xFUL /*<< SWO_PIDR4_DES_2_Pos*/)          /*!< SWO PIDR4: JEP106 continuation Mask */
 
#define SWO_PIDR2_REVISION_Pos              4U                                         /*!< SWO PIDR2: Revision Position */
#define SWO_PIDR2_REVISION_Msk             (0xFUL << SWO_PIDR2_REVISION_Pos)           /*!< SWO PIDR2: Revision Mask */
 
#define SWO_PIDR2_JEDEC_Pos                 3U                                         /*!< SWO PIDR2: JEDEC assigned Position */
#define SWO_PIDR2_JEDEC_Msk                (0x1UL << SWO_PIDR2_JEDEC_Pos)              /*!< SWO PIDR2: JEDEC assigned Mask */
 
#define SWO_PIDR2_DES_1_Pos                 0U                                         /*!< SWO PIDR2: JEP106 identity high Position */
#define SWO_PIDR2_DES_1_Msk                (0x7UL /*<< SWO_PIDR2_DES_1_Pos*/)          /*!< SWO PIDR2: JEP106 identity high Mask */
 
#define SWO_PIDR1_DES_0_Pos                 4U                                         /*!< SWO PIDR1: JEP106 identity low Position */
#define SWO_PIDR1_DES_0_Msk                (0xFUL << SWO_PIDR1_DES_0_Pos)              /*!< SWO PIDR1: JEP106 identity low Mask */
 
#define SWO_PIDR1_PART_1_Pos                0U                                         /*!< SWO PIDR1: Part number high Position */
#define SWO_PIDR1_PART_1_Msk               (0xFUL /*<< SWO_PIDR1_PART_1_Pos*/)         /*!< SWO PIDR1: Part number high Mask */
 
#define SWO_PIDR0_PART_0_Pos                0U                                         /*!< SWO PIDR0: Part number low Position */
#define SWO_PIDR0_PART_0_Msk               (0xFFUL /*<< SWO_PIDR0_PART_0_Pos*/)        /*!< SWO PIDR0: Part number low Mask */
 
/* SWO Component Identification Register Definitions */
#define SWO_CIDR1_CLASS_Pos                 4U                                         /*!< SWO CIDR1: Component class Position */
#define SWO_CIDR1_CLASS_Msk                (0xFUL << SWO_CIDR1_CLASS_Pos)              /*!< SWO CIDR1: Component class Mask */
 
#define SWO_CIDR0_PREAMBLE_0                0x0DUL                                     /*!< SWO CIDR0: expected preamble */
#define SWO_CIDR2_PREAMBLE_2                0x05UL                                     /*!< SWO CIDR2: expected preamble */
#define SWO_CIDR3_PREAMBLE_3                0xB1UL                                     /*!< SWO CIDR3: expected preamble */
#define SWO_CIDR_CLASS_CoreSight            0x9UL                                      /*!< SWO CIDR1: CoreSight component class */


/* ------------------------------------------------------------------------- */
/*    H723's TPIU                                                            */
/* ------------------------------------------------------------------------- */

/**
 * @brief Very close to CMSIS's core_cm7.h TPI, and some comments there are naming "TPIU"
 * but it looks actually different. Like H7 did built its own way for TPIU in silicon
 */
typedef struct
{
  __IM  uint32_t SUPPSIZE;              /*!< Offset: 0x000 (R/ ) Supported Port Sizes       */
  __IOM uint32_t CURPSIZE;              /*!< Offset: 0x004 (R/W) Current Port Size          */
        uint32_t RESERVED0[62U];        /*        0x008..0x0FC                         */
  __IM  uint32_t SUPTRGM;               /*!< 0x100 Supported trigger modes             */
  __IOM uint32_t TRGCNT;                /*!< 0x104 Trigger counter value               */
  __IOM uint32_t TRGMULT;               /*!< 0x108 Trigger multiplier                  */
        uint32_t RESERVED1[61U];        /*        0x10C..0x1FC                         */
  __IM  uint32_t SUPTPM;                /*!< 0x200 Supported test patterns/modes       */
  __IOM uint32_t CURTPM;                /*!< 0x204 Current test pattern/mode           */
  __IOM uint32_t TPRCR;                 /*!< 0x208 Test pattern repeat counter         */
        uint32_t RESERVED2[61U];        /*        0x20C..0x2FC                         */
  __IM  uint32_t FFSR;                  /*!< 0x300 Formatter and flush status          */
  __IOM uint32_t FFCR;                  /*!< 0x304 Formatter and flush control         */
  __IOM uint32_t FSCR;                  /*!< 0x308 Formatter synchronization counter   */
        uint32_t RESERVED3[805U];       /*        0x30C..0xF9C                         */
  __IOM uint32_t CLAIMSET;              /*!< 0xFA0 Claim tag set                       */
  __IOM uint32_t CLAIMCLR;              /*!< 0xFA4 Claim tag clear                     */
        uint32_t RESERVED5[2U];         /*        0xFA8..0xFAC                         */
  __OM  uint32_t LAR;                   /*!< 0xFB0 Lock access                         */
  __IM  uint32_t LSR;                   /*!< 0xFB4 Lock status                         */
  __IM  uint32_t AUTHSTAT;              /*!< 0xFB8 Authentication status               */
        uint32_t RESERVED6[3U];         /*        0xFBC..0xFC4                         */
  __IM  uint32_t DEVID;                 /*!< 0xFC8 Device configuration                */
  __IM  uint32_t DEVTYPE;               /*!< 0xFCC Device type identifier              */
  __IM  uint32_t PIDR4;                 /*!< 0xFD0 Peripheral ID 4                     */
        uint32_t RESERVED7[3U];         /*        0xFD4..0xFDC                         */
  __IM  uint32_t PIDR0;                 /*!< 0xFE0 Peripheral ID 0                     */
  __IM  uint32_t PIDR1;                 /*!< 0xFE4 Peripheral ID 1                     */
  __IM  uint32_t PIDR2;                 /*!< 0xFE8 Peripheral ID 2                     */
  __IM  uint32_t PIDR3;                 /*!< 0xFEC Peripheral ID 3                     */
  __IM  uint32_t CIDR0;                 /*!< 0xFF0 Component ID 0                      */
  __IM  uint32_t CIDR1;                 /*!< 0xFF4 Component ID 1                      */
  __IM  uint32_t CIDR2;                 /*!< 0xFF8 Component ID 2                      */
  __IM  uint32_t CIDR3;                 /*!< 0xFFC Component ID 3                      */
} TPIU_H7_Type;

/* ---- TPIU_SUPPSIZE / TPIU_CURPSIZE (0x000 / 0x004) --------------------- */
/* bit (n-1) set => port size n. SUPPSIZE reset 0x0000000F (sizes 1..4).     */
#define TPIU_PORTSIZE_Pos                   0U                                         /*!< TPIU xxxPSIZE: PORTSIZE Position */
#define TPIU_PORTSIZE_Msk                  (0xFFFFFFFFUL << TPIU_PORTSIZE_Pos)          /*!< TPIU xxxPSIZE: PORTSIZE Mask */
#define TPIU_PORTSIZE_WIDTH(n)             (1UL << ((n) - 1U))                          /*!< TPIU xxxPSIZE: helper, size n */

/* ---- TPIU_FFSR  (0x300) ------------------------------------------------ */
#define TPIU_FFSR_FLINPROG_Pos              0U                                         /*!< TPIU FFSR: Flush in progress Position */
#define TPIU_FFSR_FLINPROG_Msk             (0x1UL << TPIU_FFSR_FLINPROG_Pos)           /*!< TPIU FFSR: Flush in progress Mask */
#define TPIU_FFSR_FTSTOPPED_Pos             1U                                         /*!< TPIU FFSR: Formatter stopped Position */
#define TPIU_FFSR_FTSTOPPED_Msk            (0x1UL << TPIU_FFSR_FTSTOPPED_Pos)           /*!< TPIU FFSR: Formatter stopped Mask */
#define TPIU_FFSR_TCPRESENT_Pos             2U                                         /*!< TPIU FFSR: TRACECTL present Position */
#define TPIU_FFSR_TCPRESENT_Msk            (0x1UL << TPIU_FFSR_TCPRESENT_Pos)           /*!< TPIU FFSR: TRACECTL present Mask */

/* ---- TPIU_FFCR  (0x304, reset 0x0) ------------------------------------ */
#define TPIU_FFCR_ENFTC_Pos                 0U                                         /*!< TPIU FFCR: Enable formatting Position */
#define TPIU_FFCR_ENFTC_Msk                (0x1UL << TPIU_FFCR_ENFTC_Pos)              /*!< TPIU FFCR: Enable formatting Mask */
#define TPIU_FFCR_ENFCONT_Pos               1U                                         /*!< TPIU FFCR: Continuous formatting Position */
#define TPIU_FFCR_ENFCONT_Msk              (0x1UL << TPIU_FFCR_ENFCONT_Pos)            /*!< TPIU FFCR: Continuous formatting Mask */
#define TPIU_FFCR_FONFLIN_Pos               4U                                         /*!< TPIU FFCR: Flush on flush-in Position */
#define TPIU_FFCR_FONFLIN_Msk              (0x1UL << TPIU_FFCR_FONFLIN_Pos)            /*!< TPIU FFCR: Flush on flush-in Mask */
#define TPIU_FFCR_FONTRIG_Pos               5U                                         /*!< TPIU FFCR: Flush on trigger event Position */
#define TPIU_FFCR_FONTRIG_Msk              (0x1UL << TPIU_FFCR_FONTRIG_Pos)            /*!< TPIU FFCR: Flush on trigger event Mask */
#define TPIU_FFCR_FONMAN_Pos                6U                                         /*!< TPIU FFCR: Manual flush Position */
#define TPIU_FFCR_FONMAN_Msk               (0x1UL << TPIU_FFCR_FONMAN_Pos)             /*!< TPIU FFCR: Manual flush Mask */
#define TPIU_FFCR_TRIGIN_Pos                8U                                         /*!< TPIU FFCR: Trigger on TRIGIN Position */
#define TPIU_FFCR_TRIGIN_Msk               (0x1UL << TPIU_FFCR_TRIGIN_Pos)             /*!< TPIU FFCR: Trigger on TRIGIN Mask */
#define TPIU_FFCR_TRIGEVT_Pos               9U                                         /*!< TPIU FFCR: Trigger on trigger event Position */
#define TPIU_FFCR_TRIGEVT_Msk              (0x1UL << TPIU_FFCR_TRIGEVT_Pos)            /*!< TPIU FFCR: Trigger on trigger event Mask */
#define TPIU_FFCR_TRIGFL_Pos                10U                                        /*!< TPIU FFCR: Trigger on flush Position */
#define TPIU_FFCR_TRIGFL_Msk               (0x1UL << TPIU_FFCR_TRIGFL_Pos)             /*!< TPIU FFCR: Trigger on flush Mask */
#define TPIU_FFCR_STOPFL_Pos                12U                                        /*!< TPIU FFCR: Stop on flush Position */
#define TPIU_FFCR_STOPFL_Msk               (0x1UL << TPIU_FFCR_STOPFL_Pos)             /*!< TPIU FFCR: Stop on flush Mask */
#define TPIU_FFCR_STOPTRIG_Pos              13U                                        /*!< TPIU FFCR: Stop on trigger event Position */
#define TPIU_FFCR_STOPTRIG_Msk             (0x1UL << TPIU_FFCR_STOPTRIG_Pos)           /*!< TPIU FFCR: Stop on trigger event Mask */

/* ---- TPIU_FSCR  (0x400, reset 0x40 = sync every 1024 bytes) ----------- */
#define TPIU_FSCR_CYCCOUNT_Pos              0U                                         /*!< TPIU FSCR: Cycle count Position */
#define TPIU_FSCR_CYCCOUNT_Msk             (0xFFFUL << TPIU_FSCR_CYCCOUNT_Pos)         /*!< TPIU FSCR: Cycle count Mask */

/* ---- TPIU_CURTPM  (0x204) test-pattern generator ---------------------- */
#define TPIU_CURTPM_PATW1_Pos               0U                                         /*!< TPIU CURTPM: Walking 1s pattern Position */
#define TPIU_CURTPM_PATW1_Msk              (0x1UL << TPIU_CURTPM_PATW1_Pos)            /*!< TPIU CURTPM: Walking 1s pattern Mask */
#define TPIU_CURTPM_PATW0_Pos               1U                                         /*!< TPIU CURTPM: Walking 0s pattern Position */
#define TPIU_CURTPM_PATW0_Msk              (0x1UL << TPIU_CURTPM_PATW0_Pos)            /*!< TPIU CURTPM: Walking 0s pattern Mask */
#define TPIU_CURTPM_PATA5_Pos               2U                                         /*!< TPIU CURTPM: AA/55 pattern Position */
#define TPIU_CURTPM_PATA5_Msk              (0x1UL << TPIU_CURTPM_PATA5_Pos)            /*!< TPIU CURTPM: AA/55 pattern Mask */
#define TPIU_CURTPM_PATF0_Pos               3U                                         /*!< TPIU CURTPM: FF/00 pattern Position */
#define TPIU_CURTPM_PATF0_Msk              (0x1UL << TPIU_CURTPM_PATF0_Pos)            /*!< TPIU CURTPM: FF/00 pattern Mask */
#define TPIU_CURTPM_PTIMEEN_Pos             16U                                        /*!< TPIU CURTPM: Timed mode enable Position */
#define TPIU_CURTPM_PTIMEEN_Msk            (0x1UL << TPIU_CURTPM_PTIMEEN_Pos)          /*!< TPIU CURTPM: Timed mode enable Mask */
#define TPIU_CURTPM_PCONTEN_Pos             17U                                        /*!< TPIU CURTPM: Continuous mode enable Position */
#define TPIU_CURTPM_PCONTEN_Msk            (0x1UL << TPIU_CURTPM_PCONTEN_Pos)          /*!< TPIU CURTPM: Continuous mode enable Mask */

/* ---- TPIU_LSR  (0xFB4) ------------------------------------------------- */
#define TPIU_LSR_LOCKEXIST_Pos              0U                                         /*!< TPIU LSR: Lock control exists Position */
#define TPIU_LSR_LOCKEXIST_Msk             (0x1UL << TPIU_LSR_LOCKEXIST_Pos)           /*!< TPIU LSR: Lock control exists Mask */
#define TPIU_LSR_LOCKGRANT_Pos              1U                                         /*!< TPIU LSR: Lock granted Position */
#define TPIU_LSR_LOCKGRANT_Msk             (0x1UL << TPIU_LSR_LOCKGRANT_Pos)           /*!< TPIU LSR: Lock granted Mask */
#define TPIU_LSR_LOCKTYPE_Pos               2U                                         /*!< TPIU LSR: Lock size Position */
#define TPIU_LSR_LOCKTYPE_Msk              (0x1UL << TPIU_LSR_LOCKTYPE_Pos)            /*!< TPIU LSR: Lock size Mask */ 

/* ------------------------------------------------------------------------- */
/*    ETF (named ETM by STM and TMC by ARM's CoreSight)                      */
/*    ETF is a design configuration of CoreSight's Trace Memory Controller   */
/* ------------------------------------------------------------------------- */

typedef struct
{
        uint32_t RESERVED0[1U];
  __IM  uint32_t RSZ;                   /*!< Offset: 0x004 RAM Size (32-bit words)    */
        uint32_t RESERVED1[1U];
  __IM  uint32_t STS;                   /*!< Offset: 0x00C Status                     */
  __IM  uint32_t RRD;                   /*!< Offset: 0x010 RAM Read Data              */
  __IOM uint32_t RRP;                   /*!< Offset: 0x014 RAM Read Pointer           */
  __IOM uint32_t RWP;                   /*!< Offset: 0x018 RAM Write Pointer          */
  __IOM uint32_t TRG;                   /*!< Offset: 0x01C Trigger Counter            */
  __IOM uint32_t CTL;                   /*!< Offset: 0x020 Control                    */
  __OM  uint32_t RWD;                   /*!< Offset: 0x024 RAM Write Data             */
  __IOM uint32_t MODE;                  /*!< Offset: 0x028 Mode                       */
  __IM  uint32_t LBUFLEVEL;             /*!< Offset: 0x02C Latched Buffer Fill Level  */
  __IM  uint32_t CBUFLEVEL;             /*!< Offset: 0x030 Current Buffer Fill Level  */
  __IOM uint32_t BUFWM;                 /*!< Offset: 0x034 Buffer Level Watermark     */
  __IOM uint32_t RRPHI;                 /*!< Offset: 0x038 RAM Read Pointer High      */
  __IOM uint32_t RWPHI;                 /*!< Offset: 0x03C RAM Write Pointer High     */
        uint32_t RESERVED2[52U];
  __IOM uint32_t AXICTL;                /*!< Offset: 0x110 AXI Control                */
        uint32_t RESERVED3[1U];
  __IOM uint32_t DBALO;                 /*!< Offset: 0x118 Data Buffer Address Low    */
  __IOM uint32_t DBAHI;                 /*!< Offset: 0x11C Data Buffer Address High   */
        uint32_t RESERVED4[120U];
  __IM  uint32_t FFSR;                  /*!< Offset: 0x300 Formatter and Flush Status */
  __IOM uint32_t FFCR;                  /*!< Offset: 0x304 Formatter and Flush Control*/
  __IOM uint32_t PSCR;                  /*!< Offset: 0x308 Periodic Sync Counter      */
        uint32_t RESERVED5[809U];
  __OM  uint32_t LAR;                   /*!< Offset: 0xFB0 Lock Access                */
  __IM  uint32_t LSR;                   /*!< Offset: 0xFB4 Lock Status                */
} TMC_Type;

#define TMC_STS_Full_Pos                    0U
#define TMC_STS_Full_Msk                   (0x1UL << TMC_STS_Full_Pos)

#define TMC_STS_TMCReady_Pos                2U
#define TMC_STS_TMCReady_Msk               (0x1UL << TMC_STS_TMCReady_Pos)

#define TMC_CTL_TraceCaptEn_Pos             0U
#define TMC_CTL_TraceCaptEn_Msk            (0x1UL << TMC_CTL_TraceCaptEn_Pos)

#define TMC_MODE_MODE_Pos                   0U
#define TMC_MODE_MODE_Msk                  (0x3UL << TMC_MODE_MODE_Pos)
#define TMC_MODE_CIRCULAR                  (0x0UL)
#define TMC_MODE_SW_FIFO                   (0x1UL)
#define TMC_MODE_HW_FIFO                   (0x2UL)

#define TMC_FFCR_EnFT_Pos                   0U
#define TMC_FFCR_EnFT_Msk                  (0x1UL << TMC_FFCR_EnFT_Pos)   /*!< enable formatting */

#define TMC_FFCR_EnTI_Pos                   1U
#define TMC_FFCR_EnTI_Msk                  (0x1UL << TMC_FFCR_EnTI_Pos)   /*!< embed triggers    */

#define TMC_FFCR_FOnFlIn_Pos                4U
#define TMC_FFCR_FOnFlIn_Msk               (0x1UL << TMC_FFCR_FOnFlIn_Pos)

#define TMC_FFCR_FOnTrig_Pos                5U
#define TMC_FFCR_FOnTrig_Msk               (0x1UL << TMC_FFCR_FOnTrig_Pos)

#define TMC_FFCR_FlushMan_Pos               6U
#define TMC_FFCR_FlushMan_Msk              (0x1UL << TMC_FFCR_FlushMan_Pos)

#define TMC_FFCR_StopOnFl_Pos               12U
#define TMC_FFCR_StopOnFl_Msk              (0x1UL << TMC_FFCR_StopOnFl_Pos)

#define TMC_FFSR_FlInProg_Pos               0U
#define TMC_FFSR_FlInProg_Msk              (0x1UL << TMC_FFSR_FlInProg_Pos)
#define TMC_RRD_DRAINED                    (0xFFFFFFFFUL) /*!< returned once RAM is emptied */

/* ------------------------------------------------------------------------- */
/*    ETMv4                                                            */
/* ------------------------------------------------------------------------- */

typedef struct
{
        uint32_t RESERVED0[1U];         /*        0x000                             */
  __IOM uint32_t PRGCTL;                /*!< 0x004 Programming control              */
  __IM  uint32_t PROCSEL;               /*!< 0x008 Processor select                 */
  __IM  uint32_t STAT;                  /*!< 0x00C Status                           */
  __IOM uint32_t CONFIG;                /*!< 0x010 Trace configuration              */
        uint32_t RESERVED1[3U];         /*        0x014..0x01C                      */
  __IOM uint32_t EVENTCTL0;             /*!< 0x020 Event control 0                  */
  __IOM uint32_t EVENTCTL1;             /*!< 0x024 Event control 1                  */
        uint32_t RESERVED2[1U];         /*        0x028                             */
  __IOM uint32_t STALLCTL;              /*!< 0x02C Stall control                    */
  __IOM uint32_t TSCTL;                 /*!< 0x030 Global timestamp control         */
  __IM  uint32_t SYNCP;                 /*!< 0x034 Sync period (read-only here)     */
  __IOM uint32_t CCCTL;                 /*!< 0x038 Cycle count control              */
        uint32_t RESERVED3[1U];         /*        0x03C                             */
  __IOM uint32_t TRACEID;               /*!< 0x040 Trace ID                         */
        uint32_t RESERVED4[15U];        /*        0x044..0x07C                      */
  __IOM uint32_t VICTL;                 /*!< 0x080 ViewInst main control            */
        uint32_t RESERVED5[1U];         /*        0x084  (no VIIECTLR here)         */
  __IOM uint32_t VISSCTL;               /*!< 0x088 ViewInst start/stop control      */
  __IOM uint32_t VIPCSSCTL;             /*!< 0x08C ViewInst start/stop PE comparator*/
        uint32_t RESERVED6[44U];        /*        0x090..0x13C                      */
  __IOM uint32_t CNTRLDV;               /*!< 0x140 Counter reload value             */
        uint32_t RESERVED7[15U];        /*        0x144..0x17C                      */
  __IM  uint32_t IDR8;                  /*!< 0x180 ID 8  (MAXSPEC)                  */
  __IM  uint32_t IDR9;                  /*!< 0x184 ID 9                             */
  __IM  uint32_t IDR10;                 /*!< 0x188 ID 10                            */
  __IM  uint32_t IDR11;                 /*!< 0x18C ID 11                            */
  __IM  uint32_t IDR12;                 /*!< 0x190 ID 12                            */
  __IM  uint32_t IDR13;                 /*!< 0x194 ID 13                            */
        uint32_t RESERVED8[10U];        /*        0x198..0x1BC                      */
  __IOM uint32_t IMSPEC0;               /*!< 0x1C0 Implementation specific 0        */
        uint32_t RESERVED9[7U];         /*        0x1C4..0x1DC                      */
  __IM  uint32_t IDR0;                  /*!< 0x1E0 ID 0                             */
  __IM  uint32_t IDR1;                  /*!< 0x1E4 ID 1  (0x4100F401 = ETMv4)       */
  __IM  uint32_t IDR2;                  /*!< 0x1E8 ID 2                             */
  __IM  uint32_t IDR3;                  /*!< 0x1EC ID 3  (CCITMIN)                  */
  __IM  uint32_t IDR4;                  /*!< 0x1F0 ID 4                             */
  __IM  uint32_t IDR5;                  /*!< 0x1F4 ID 5                             */
        uint32_t RESERVED10[4U];        /*        0x1F8..0x204                      */
  __IOM uint32_t RSCTL2;                /*!< 0x208 Resource selection 2             */
  __IOM uint32_t RSCTL3;                /*!< 0x20C Resource selection 3             */
        uint32_t RESERVED11[28U];       /*        0x210..0x27C                      */
  __IOM uint32_t SSCC0;                 /*!< 0x280 Single-shot comparator control 0 */
        uint32_t RESERVED12[7U];        /*        0x284..0x29C                      */
  __IOM uint32_t SSCS0;                 /*!< 0x2A0 Single-shot comparator status 0  */
        uint32_t RESERVED13[7U];        /*        0x2A4..0x2BC                      */
  __IOM uint32_t SSPCIC0;               /*!< 0x2C0 Single-shot PE comparator input  */
        uint32_t RESERVED14[19U];       /*        0x2C4..0x30C  (no OSLAR at 0x300) */
  __IOM uint32_t PDC;                   /*!< 0x310 Power-down control               */
  __IM  uint32_t PDS;                   /*!< 0x314 Power-down status                */
        uint32_t RESERVED15[802U];      /*        0x318..0xF9C                      */
  __IOM uint32_t CLAIMSET;              /*!< 0xFA0 Claim tag set                    */
  __IOM uint32_t CLAIMCLR;              /*!< 0xFA4 Claim tag clear                  */
        uint32_t RESERVED16[2U];        /*        0xFA8..0xFAC                      */
  __OM  uint32_t LAR;                   /*!< 0xFB0 Lock access                      */
  __IM  uint32_t LSR;                   /*!< 0xFB4 Lock status                      */
  __IM  uint32_t AUTHSTAT;              /*!< 0xFB8 Authentication status            */
  __IM  uint32_t DEVARCH;               /*!< 0xFBC Device architecture (0x4770 4A13)*/
        uint32_t RESERVED17[3U];        /*        0xFC0..0xFC8                      */
  __IM  uint32_t DEVTYPE;               /*!< 0xFCC Device type (0x13 trace source)  */
  __IM  uint32_t PIDR4;                 /*!< 0xFD0                                  */
        uint32_t RESERVED18[3U];        /*        0xFD4..0xFDC                      */
  __IM  uint32_t PIDR0;                 /*!< 0xFE0                                  */
  __IM  uint32_t PIDR1;                 /*!< 0xFE4                                  */
  __IM  uint32_t PIDR2;                 /*!< 0xFE8                                  */
  __IM  uint32_t PIDR3;                 /*!< 0xFEC                                  */
  __IM  uint32_t CIDR0;                 /*!< 0xFF0                                  */
  __IM  uint32_t CIDR1;                 /*!< 0xFF4                                  */
  __IM  uint32_t CIDR2;                 /*!< 0xFF8                                  */
  __IM  uint32_t CIDR3;                 /*!< 0xFFC                                  */
} ETM_Type;
 
/* ---- M7_ETM_PRGCTL  (0x004) ------------------------------------------- */
#define ETM_PRGCTL_EN_Pos                   0U                                         /*!< ETM PRGCTL: Trace program enable Position */
#define ETM_PRGCTL_EN_Msk                  (0x1UL << ETM_PRGCTL_EN_Pos)                /*!< ETM PRGCTL: Trace program enable Mask */

/* ---- M7_ETM_STAT  (0x00C) --------------------------------------------- */
#define ETM_STAT_IDLE_Pos                   0U                                         /*!< ETM STAT: Trace unit idle Position */
#define ETM_STAT_IDLE_Msk                  (0x1UL << ETM_STAT_IDLE_Pos)                /*!< ETM STAT: Trace unit idle Mask */
#define ETM_STAT_PMSTABLE_Pos               1U                                         /*!< ETM STAT: Programmers model stable Position */
#define ETM_STAT_PMSTABLE_Msk              (0x1UL << ETM_STAT_PMSTABLE_Pos)            /*!< ETM STAT: Programmers model stable Mask */

/* ---- M7_ETM_CONFIG  (0x010) ------------------------------------------- */
#define ETM_CONFIG_BB_Pos                   3U                                         /*!< ETM CONFIG: Branch broadcast Position */
#define ETM_CONFIG_BB_Msk                  (0x1UL << ETM_CONFIG_BB_Pos)                /*!< ETM CONFIG: Branch broadcast Mask */
#define ETM_CONFIG_CCI_Pos                  4U                                         /*!< ETM CONFIG: Cycle counting in instr trace Position */
#define ETM_CONFIG_CCI_Msk                 (0x1UL << ETM_CONFIG_CCI_Pos)               /*!< ETM CONFIG: Cycle counting in instr trace Mask */
#define ETM_CONFIG_COND_Pos                 8U                                         /*!< ETM CONFIG: Conditional instr tracing Position */
#define ETM_CONFIG_COND_Msk                (0x7UL << ETM_CONFIG_COND_Pos)              /*!< ETM CONFIG: Conditional instr tracing Mask */
#define ETM_CONFIG_TS_Pos                   11U                                        /*!< ETM CONFIG: Global timestamp tracing Position */
#define ETM_CONFIG_TS_Msk                  (0x1UL << ETM_CONFIG_TS_Pos)                /*!< ETM CONFIG: Global timestamp tracing Mask */
#define ETM_CONFIG_RS_Pos                   12U                                        /*!< ETM CONFIG: Return stack enable Position */
#define ETM_CONFIG_RS_Msk                  (0x1UL << ETM_CONFIG_RS_Pos)                /*!< ETM CONFIG: Return stack enable Mask */
#define ETM_CONFIG_DA_Pos                   16U                                        /*!< ETM CONFIG: Data address tracing (RO) Position */
#define ETM_CONFIG_DA_Msk                  (0x1UL << ETM_CONFIG_DA_Pos)                /*!< ETM CONFIG: Data address tracing (RO) Mask */
#define ETM_CONFIG_DV_Pos                   17U                                        /*!< ETM CONFIG: Data value tracing (RO) Position */
#define ETM_CONFIG_DV_Msk                  (0x1UL << ETM_CONFIG_DV_Pos)                /*!< ETM CONFIG: Data value tracing (RO) Mask */

/* ---- M7_ETM_STALLCTL  (0x02C) ----------------------------------------- */
#define ETM_STALLCTL_LEVEL_Pos              2U                                         /*!< ETM STALLCTL: Stall threshold level Position */
#define ETM_STALLCTL_LEVEL_Msk             (0x3UL << ETM_STALLCTL_LEVEL_Pos)           /*!< ETM STALLCTL: Stall threshold level Mask */
#define ETM_STALLCTL_ISTALL_Pos             8U                                         /*!< ETM STALLCTL: Instr-buffer stall enable Position */
#define ETM_STALLCTL_ISTALL_Msk            (0x1UL << ETM_STALLCTL_ISTALL_Pos)          /*!< ETM STALLCTL: Instr-buffer stall enable Mask */
#define ETM_STALLCTL_DSTALL_Pos             9U                                         /*!< ETM STALLCTL: Data-buffer stall enable Position */
#define ETM_STALLCTL_DSTALL_Msk            (0x1UL << ETM_STALLCTL_DSTALL_Pos)          /*!< ETM STALLCTL: Data-buffer stall enable Mask */

/* ---- M7_ETM_TSCTL  (0x030) -------------------------------------------- */
#define ETM_TSCTL_SEL_Pos                   0U                                         /*!< ETM TSCTL: Resource selector Position */
#define ETM_TSCTL_SEL_Msk                  (0xFUL << ETM_TSCTL_SEL_Pos)                /*!< ETM TSCTL: Resource selector Mask */
#define ETM_TSCTL_TYPE_Pos                  7U                                         /*!< ETM TSCTL: Resource type Position */
#define ETM_TSCTL_TYPE_Msk                 (0x1UL << ETM_TSCTL_TYPE_Pos)               /*!< ETM TSCTL: Resource type Mask */

/* ---- M7_ETM_SYNCP  (0x034, read-only, reset 0xA = 1024 bytes) --------- */
#define ETM_SYNCP_PERIOD_Pos                0U                                         /*!< ETM SYNCP: Sync period Position */
#define ETM_SYNCP_PERIOD_Msk               (0x1FUL << ETM_SYNCP_PERIOD_Pos)            /*!< ETM SYNCP: Sync period Mask */

/* ---- M7_ETM_CCCTL  (0x038) -------------------------------------------- */
#define ETM_CCCTL_THRESHOLD_Pos             0U                                         /*!< ETM CCCTL: Cycle-count threshold Position */
#define ETM_CCCTL_THRESHOLD_Msk            (0xFFFUL << ETM_CCCTL_THRESHOLD_Pos)        /*!< ETM CCCTL: Cycle-count threshold Mask */

/* ---- M7_ETM_TRACEID  (0x040) ------------------------------------------ */
#define ETM_TRACEID_TRACEID_Pos             0U                                         /*!< ETM TRACEID: Trace ID Position */
#define ETM_TRACEID_TRACEID_Msk            (0x7FUL << ETM_TRACEID_TRACEID_Pos)         /*!< ETM TRACEID: Trace ID Mask */

/* ---- M7_ETM_VICTL  (0x080) -------------------------------------------- */
#define ETM_VICTL_SEL_Pos                   0U                                         /*!< ETM VICTL: Resource selector Position */
#define ETM_VICTL_SEL_Msk                  (0xFUL << ETM_VICTL_SEL_Pos)                /*!< ETM VICTL: Resource selector Mask */
#define ETM_VICTL_TYPE_Pos                  7U                                         /*!< ETM VICTL: Resource type Position */
#define ETM_VICTL_TYPE_Msk                 (0x1UL << ETM_VICTL_TYPE_Pos)               /*!< ETM VICTL: Resource type Mask */
#define ETM_VICTL_SSSTATUS_Pos              9U                                         /*!< ETM VICTL: Start/stop logic status Position */
#define ETM_VICTL_SSSTATUS_Msk             (0x1UL << ETM_VICTL_SSSTATUS_Pos)           /*!< ETM VICTL: Start/stop logic status Mask */
#define ETM_VICTL_TRCRESET_Pos              10U                                        /*!< ETM VICTL: Trace reset exception Position */
#define ETM_VICTL_TRCRESET_Msk             (0x1UL << ETM_VICTL_TRCRESET_Pos)           /*!< ETM VICTL: Trace reset exception Mask */
#define ETM_VICTL_TRCERR_Pos                11U                                        /*!< ETM VICTL: Trace system error exception Position */
#define ETM_VICTL_TRCERR_Msk               (0x1UL << ETM_VICTL_TRCERR_Pos)             /*!< ETM VICTL: Trace system error exception Mask */
#define ETM_VICTL_EXLEVEL_S0_Pos            16U                                        /*!< ETM VICTL: Trace disable EL0 secure Position */
#define ETM_VICTL_EXLEVEL_S0_Msk           (0x1UL << ETM_VICTL_EXLEVEL_S0_Pos)         /*!< ETM VICTL: Trace disable EL0 secure Mask */
#define ETM_VICTL_EXLEVEL_S3_Pos            19U                                        /*!< ETM VICTL: Trace disable EL3 secure Position */
#define ETM_VICTL_EXLEVEL_S3_Msk           (0x1UL << ETM_VICTL_EXLEVEL_S3_Pos)         /*!< ETM VICTL: Trace disable EL3 secure Mask */
/* Resource selector 1 is architecturally always-true; select it to trace all. */
#define ETM_VICTL_RESOURCE_TRUE            (0x1UL)                                      /*!< ETM VICTL: SEL value for always-true resource */

/* ---- M7_ETM_IDR3  (0x1EC) --------------------------------------------- */
#define ETM_IDR3_CCITMIN_Pos                0U                                         /*!< ETM IDR3: Min cycle-count threshold Position */
#define ETM_IDR3_CCITMIN_Msk               (0xFFFUL << ETM_IDR3_CCITMIN_Pos)           /*!< ETM IDR3: Min cycle-count threshold Mask */

/* ---- M7_ETM_PDS  (0x314) ---------------------------------------------- */
#define ETM_PDS_POWER_Pos                   0U                                         /*!< ETM PDS: ETM powered up Position */
#define ETM_PDS_POWER_Msk                  (0x1UL << ETM_PDS_POWER_Pos)                /*!< ETM PDS: ETM powered up Mask */
#define ETM_PDS_STICKYPD_Pos                1U                                         /*!< ETM PDS: Sticky power-down Position */
#define ETM_PDS_STICKYPD_Msk               (0x1UL << ETM_PDS_STICKYPD_Pos)             /*!< ETM PDS: Sticky power-down Mask */

/* ------------------------------------------------------------------------- */
/*    CSTF (CoreSight Trace Funnel)                                          */
/* ------------------------------------------------------------------------- */

typedef struct {
  __IOM uint32_t CTRL;                  /*!< 0x000 Funnel control       (reset 0x0300) */
  __IOM uint32_t PRIORITY;              /*!< 0x004 Port priority        (reset 0x0688) */
        uint32_t RESERVED0[998U];       /*        0x008..0xF9C                         */
  __IOM uint32_t CLAIMSET;              /*!< 0xFA0 Claim tag set        (reset 0x000F) */
  __IOM uint32_t CLAIMCLR;              /*!< 0xFA4 Claim tag clear                     */
        uint32_t RESERVED1[2U];         /*        0xFA8..0xFAC                         */
  __OM  uint32_t LAR;                   /*!< 0xFB0 Lock access                         */
  __IM  uint32_t LSR;                   /*!< 0xFB4 Lock status          (reset 0x0003) */
  __IM  uint32_t AUTHSTAT;              /*!< 0xFB8 Authentication status(reset 0x000A) */
        uint32_t RESERVED2[3U];         /*        0xFBC..0xFC4                         */
  __IM  uint32_t DEVID;                 /*!< 0xFC8 Device ID            (reset 0x0024) */
  __IM  uint32_t DEVTYPE;               /*!< 0xFCC Device type          (reset 0x0012) */
  __IM  uint32_t PIDR4;                 /*!< 0xFD0                      (reset 0x0004) */
        uint32_t RESERVED3[3U];         /*        0xFD4..0xFDC                         */
  __IM  uint32_t PIDR0;                 /*!< 0xFE0                      (reset 0x0008) */
  __IM  uint32_t PIDR1;                 /*!< 0xFE4                      (reset 0x00B9) */
  __IM  uint32_t PIDR2;                 /*!< 0xFE8                      (reset 0x003B) */
  __IM  uint32_t PIDR3;                 /*!< 0xFEC                      (reset 0x0000) */
  __IM  uint32_t CIDR0;                 /*!< 0xFF0                      (reset 0x000D) */
  __IM  uint32_t CIDR1;                 /*!< 0xFF4                      (reset 0x0090) */
  __IM  uint32_t CIDR2;                 /*!< 0xFF8                      (reset 0x0005) */
  __IM  uint32_t CIDR3;                 /*!< 0xFFC */
} CSTF_Type;

#define CSTF_CTRL_ENS0_Pos                  0U                                    /*!< CSTF CTRL: S0 (ETM) port enable Position */
#define CSTF_CTRL_ENS0_Msk                 (0x1UL << CSTF_CTRL_ENS0_Pos)          /*!< CSTF CTRL: S0 (ETM) port enable Mask */
#define CSTF_CTRL_ENS1_Pos                  1U                                    /*!< CSTF CTRL: S1 (ITM) port enable Position */
#define CSTF_CTRL_ENS1_Msk                 (0x1UL << CSTF_CTRL_ENS1_Pos)          /*!< CSTF CTRL: S1 (ITM) port enable Mask */
#define CSTF_CTRL_MINHOLD_Pos               8U                                    /*!< CSTF CTRL: Min hold time Position */
#define CSTF_CTRL_MINHOLD_Msk              (0xFUL << CSTF_CTRL_MINHOLD_Pos)       /*!< CSTF CTRL: Min hold time Mask */



/* ---- H7 system-level debug components (D3 debug APB) ---- */
#define SWO_BASE            (0x5C003000UL)                            /*!< Serial Wire Output Base Address */
#define ROM_BASE            (0x5C000000UL)
#define ETF_BASE            (0x5C015000UL)
#define TPIU_H7_BASE        (0x5C015000UL)   /* STM32H723's TPIU, official CoreSight is not at this address */
#define ETM_BASE            (0xE0041000UL)   /* STM32H723's ETM as ETM is optional for Cortex M */
#define CSTF_BASE           (0x5C013000UL)   /* STM32H723's CSTF CoreSight Trace Funnel */

#define SWO                 ((SWO_Type       *)     SWO_BASE      )   /*!< SWO configuration struct */
#define TPIU_H7             ((TPIU_H7_Type   *)     TPIU_H7_BASE  )
#define ETM                 ((ETM_Type       *)     ETM_BASE      )
#define ETF                 ((TMC_Type       *)     ETF_BASE      )
#define CSTF                ((CSTF_Type      *)     CSTF_BASE     )

/* Miscellaneous values */
#define CS_KEY              0xC5ACCE55UL
#define TRACE_ID_ETM        0x20UL
#define TRACE_PORT_WIDTH    (4U)             /*!< 4-bit parallel port      */

/**
  \brief   SWO Unlock
  \details Removes the CoreSight software lock so that SWO->CODR, SWO->SPPR and
           the claim tag registers accept writes through the debug APB.
           Must be called before any configuration write.
 */
__STATIC_INLINE void SWO_Unlock (void)
{
  SWO->LAR = CS_KEY;
}


/**
  \brief   SWO Lock
  \details Re-engages the CoreSight software lock. Any value other than the
           access key locks the component.
 */
__STATIC_INLINE void SWO_Lock (void)
{
  SWO->LAR = 0UL;
}
 
 
/**
  \brief   SWO Get Lock Status
  \details Reports whether the software lock is currently engaged.
  \return             1  SWO is locked, writes are ignored.
  \return             0  SWO is unlocked.
 */
__STATIC_INLINE uint32_t SWO_GetLockStatus (void)
{
  return (_FLD2VAL(SWO_LSR_SLK, SWO->LSR));
}
 
 
/**
  \brief   SWO Is Lock Implemented
  \details Reports whether the component implements a software lock at all.
  \return             1  Lock mechanism present.
  \return             0  Lock mechanism absent, writes always permitted.
 */
__STATIC_INLINE uint32_t SWO_IsLockImplemented (void)
{
  return (_FLD2VAL(SWO_LSR_SLI, SWO->LSR));
}
 
 
/**
  \brief   SWO Set Prescaler
  \details Programs the current output divisor. The resulting line rate is
           SWO_clock / (prescaler + 1), where SWO_clock is the clock feeding
           this component - on STM32H723 this is not SystemCoreClock, derive it
           from RM0468.
  \param [in]  prescaler  Divisor value, 0 .. 0x1FFF.
 */
__STATIC_INLINE void SWO_SetPrescaler (uint32_t prescaler)
{
  SWO->CODR = _VAL2FLD(SWO_CODR_PRESCALER, prescaler);
}
 
 
/**
  \brief   SWO Get Prescaler
  \details Reads back the programmed current output divisor.
  \return             Divisor value, 0 .. 0x1FFF.
 */
__STATIC_INLINE uint32_t SWO_GetPrescaler (void)
{
  return (_FLD2VAL(SWO_CODR_PRESCALER, SWO->CODR));
}
 
 
/**
  \brief   SWO Set Protocol
  \details Selects the line encoding driven onto the TRACESWO pin.
  \param [in]  protocol  \ref SWO_SPPR_TXMODE_Manchester or
                         \ref SWO_SPPR_TXMODE_NRZ.
 */
__STATIC_INLINE void SWO_SetProtocol (uint32_t protocol)
{
  SWO->SPPR = (protocol & SWO_SPPR_TXMODE_Msk);
}
 
 
/**
  \brief   SWO Get Protocol
  \details Reads back the selected line encoding.
  \return             Encoded TXMODE field value.
 */
__STATIC_INLINE uint32_t SWO_GetProtocol (void)
{
  return (_FLD2VAL(SWO_SPPR_TXMODE, SWO->SPPR));
}
 
 
/**
  \brief   SWO Get Trace Capture Device Status
  \details Reports whether a trace capture device is asserting its presence.
  \return             1  Capture device present.
  \return             0  No capture device detected.
 */
__STATIC_INLINE uint32_t SWO_IsTraceCaptureDevicePresent (void)
{
  return (_FLD2VAL(SWO_FFSR_TCPresent, SWO->FFSR));
}
 
 
/**
  \brief   SWO Get Flush Status
  \details Reports whether a flush operation is currently in progress.
  \return             1  Flush in progress.
  \return             0  Idle.
 */
__STATIC_INLINE uint32_t SWO_IsFlushInProgress (void)
{
  return (_FLD2VAL(SWO_FFSR_FlInProg, SWO->FFSR));
}
 
 
/**
  \brief   SWO Get Formatter Stopped Status
  \details Reports whether the formatter has stopped and all trace data has
           been drained.
  \return             1  Formatter stopped.
  \return             0  Formatter running.
 */
__STATIC_INLINE uint32_t SWO_IsFormatterStopped (void)
{
  return (_FLD2VAL(SWO_FFSR_FtStopped, SWO->FFSR));
}
 
 
/**
  \brief   SWO Set Claim Tag
  \details Asserts claim tag bits, used to negotiate ownership of the component
           between debug agents.
  \param [in]  tags  Bitmask of tags to assert, 0 .. 0xF.
 */
__STATIC_INLINE void SWO_SetClaimTag (uint32_t tags)
{
  SWO->CLAIMSET = (tags & SWO_CLAIM_TAG_Msk);
}
 
 
/**
  \brief   SWO Clear Claim Tag
  \details Releases claim tag bits.
  \param [in]  tags  Bitmask of tags to release, 0 .. 0xF.
 */
__STATIC_INLINE void SWO_ClearClaimTag (uint32_t tags)
{
  SWO->CLAIMCLR = (tags & SWO_CLAIM_TAG_Msk);
}
 
 
/**
  \brief   SWO Get Claim Tag
  \details Reads the currently asserted claim tags.
  \return             Bitmask of asserted tags.
 */
__STATIC_INLINE uint32_t SWO_GetClaimTag (void)
{
  return (SWO->CLAIMCLR & SWO_CLAIM_TAG_Msk);
}
 
 
/**
  \brief   SWO Get Supported Claim Tags
  \details Reads the number of claim tag bits the component implements.
  \return             Bitmask of implemented tags.
 */
__STATIC_INLINE uint32_t SWO_GetSupportedClaimTags (void)
{
  return (SWO->CLAIMSET & SWO_CLAIM_TAG_Msk);
}
 
 
/**
  \brief   SWO Get Part Number
  \details Reassembles the 12-bit CoreSight part number from PIDR0 and PIDR1.
  \return             Part number.
 */
__STATIC_INLINE uint32_t SWO_GetPartNumber (void)
{
  return (_FLD2VAL(SWO_PIDR0_PART_0, SWO->PIDR0) |
         (_FLD2VAL(SWO_PIDR1_PART_1, SWO->PIDR1) << 8U));
}
 
 
/**
  \brief   SWO Get Designer
  \details Reassembles the 7-bit JEP106 designer code from PIDR1 and PIDR2.
  \return             Designer code, 0x3B for ARM.
 */
__STATIC_INLINE uint32_t SWO_GetDesigner (void)
{
  return (_FLD2VAL(SWO_PIDR1_DES_0, SWO->PIDR1) |
         (_FLD2VAL(SWO_PIDR2_DES_1, SWO->PIDR2) << 4U));
}
 
 
/**
  \brief   SWO Get Revision
  \details Reads the component revision field from PIDR2.
  \return             Revision.
 */
__STATIC_INLINE uint32_t SWO_GetRevision (void)
{
  return (_FLD2VAL(SWO_PIDR2_REVISION, SWO->PIDR2));
}
 
 
/**
  \brief   SWO Get Device Major Type
  \details Reads the CoreSight major device type from DEVTYPE.
  \return             Major type.
 */
__STATIC_INLINE uint32_t SWO_GetDeviceMajorType (void)
{
  return (_FLD2VAL(SWO_DEVTYPE_MajorType, SWO->DEVTYPE));
}
 
 
/**
  \brief   SWO Get Device Sub Type
  \details Reads the CoreSight device sub type from DEVTYPE.
  \return             Sub type.
 */
__STATIC_INLINE uint32_t SWO_GetDeviceSubType (void)
{
  return (_FLD2VAL(SWO_DEVTYPE_SubType, SWO->DEVTYPE));
}


/**
  \brief   SWO Check Component Presence
  \details Validates the CoreSight component identification preamble at
           \ref SWO_BASE. Serves as a check that the D3 debug APB is clocked
           and powered before any other read-back is trusted - an unclocked
           domain typically returns all zeroes or all ones.
  \return             1  Valid CoreSight component present.
  \return             0  Preamble mismatch.
 */
__STATIC_INLINE uint32_t SWO_CheckPresence (void)
{
  if ((SWO->CIDR0 & 0xFFUL) != SWO_CIDR0_PREAMBLE_0)                     { return (0UL); }
  if ((SWO->CIDR2 & 0xFFUL) != SWO_CIDR2_PREAMBLE_2)                     { return (0UL); }
  if ((SWO->CIDR3 & 0xFFUL) != SWO_CIDR3_PREAMBLE_3)                     { return (0UL); }
  if (_FLD2VAL(SWO_CIDR1_CLASS, SWO->CIDR1) != SWO_CIDR_CLASS_CoreSight) { return (0UL); }
 
  return (1UL);
}

/**
 * @brief Send a value on channel 1  
 * 
 * @param val value to send, uint32_t
 */
__STATIC_INLINE void ITM_SendHex(uint32_t val)
{
   if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && /* ITM enabled */
       ((ITM->TER & 2UL)                != 0UL))   /* ITM Port #1 enabled */
   {
      while (ITM->PORT[1U].u32 == 0UL)
      {
         __NOP();
      }
      ITM->PORT[1U].u32 = val;
   }
}

/**
 * @brief Send a value on a channel
 * @warning, check first that channel was activated with
 * ITM->TER = 0x000F;
 * @param val value to send, uint32_t
 * @param channel which channel to send it
 */
__STATIC_INLINE void ITM_Send(uint32_t val, unsigned int channel)
{
   while (ITM->PORT[channel].u32 == 0UL)
   {
      __NOP();
   }
   ITM->PORT[channel].u32 = val;
}

#endif /* (__SWO_PRESENT == 1U) */

void initDebugBlock(void);

uint32_t configureSWO (uint32_t protocol, uint32_t SystemCoreClock, uint32_t baudrate);

bool parallel_trace_start(void);

void ETM_Disable(void);

#ifdef __cplusplus
}
#endif

#endif /* _STM32H7XX_TRACE_H_ */