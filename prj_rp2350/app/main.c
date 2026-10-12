/**
 * @file    main.c
 * @author  Jeremie Gallee (jeremie.gallee@noser.com)
 * @brief   main file for RP2350 project example with ThreadX,
 *          gdb/OpenOCD debugging and CMake project management
 * @version 0.1
 * @date    2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "board.h"
#include "app_freeRtos.h"
#include "comm_with_flaw.h"
#include "pico.h"
#include "pico/multicore.h"

/* Prototypes for the standard FreeRTOS callback/hook functions implemented within this file. */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);

int main(void)
{
   stdio_init_all();

   printf("\n --- Start program ---\n");

   comm_init();

   /* Start RTOS */
   vLaunch();

   return 0;
}

/* Heap problem at startup */
void vApplicationMallocFailedHook(void)
{
   /* Force assert. */
   configASSERT((volatile void *)NULL);
}

/* Stack problem at startup */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
   (void)pcTaskName;
   (void)pxTask;

   /* Force an assert. */
   configASSERT((volatile void *)NULL);
}

/* In IDLE, do some stuff like monitor available heap */
void vApplicationIdleHook(void)
{
   volatile size_t xFreeHeapSpace;

   /* Check available heap */
   xFreeHeapSpace = xPortGetFreeHeapSize();

   (void)xFreeHeapSpace;
}
