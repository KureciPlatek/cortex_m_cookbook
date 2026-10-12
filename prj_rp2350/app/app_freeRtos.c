/**
 * @file    app_freeRtos.c
 * @author  Jeremie Gallee (jeremie.gallee@noser.com)
 * @brief   FreeRTOS application for project prj_rp2350
 *          Creates some tasks and have fun
 * @version 0.1
 * @date    2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "board.h"
#include "app_freeRtos.h"
#include "comm_with_flaw.h"
#include "pico/platform.h"

static void prvQueueSendTask(void *pvParameters);
static void prvQueueReceiveTask(void *pvParameters);

/* @todo move to not global and provide queue only to some tasks at startup with xTaskCreate() */
static QueueHandle_t xQueue = NULL;
static bool led_state = false;

void vLaunch(void)
{
   TaskHandle_t task_rcvTask;
   TaskHandle_t task_sndTask;
   BaseType_t ret;

   xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

   if(NULL != xQueue)
   {
      if(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xTaskCreateAffinitySet(prvQueueReceiveTask, "Rx", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, TASK_AFFINITY_CORE_1, &task_rcvTask))
      {
         assert(false);
      }
      if(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY == xTaskCreateAffinitySet(prvQueueSendTask,    "Tx", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY,    TASK_AFFINITY_CORE_0, &task_sndTask))
      {
         assert(false);
      }

      vTaskStartScheduler();
   }
   else
   {
      assert(false);
   }

   while(true) {};
}

/**
 * @brief Send messages to a queue
 * @param pvParameters
 */
static void prvQueueSendTask(void *pvParameters)
{
   TickType_t xNextWakeTime;
   const unsigned long ulValueToSend = 100UL;

   (void)pvParameters; /* No params */

   xNextWakeTime = xTaskGetTickCount();

   uint core = get_core_num();

   for (;;)
   {
      vTaskDelayUntil(&xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS);
//      printf("Core_%d - Task send\n", core);
      if(pdPASS != xQueueSend(xQueue, &ulValueToSend, 0U))
      {
//         printf("Core_%d - Queue full\n", core);
      }

      comm_process_msg();
   }
}

/**
 * @brief Receive messages from Queue
 * @param pvParameters
 */
static void prvQueueReceiveTask(void *pvParameters)
{
   unsigned long ulReceivedValue;
   const unsigned long ulExpectedValue = 100UL;

   (void)pvParameters; /* No params */

   while (cyw43_arch_init() != 0)
   {
      printf("Waiting for WiFi initialization...\n");
      vTaskDelay(250);
   }

   uint core = get_core_num();

   for (;;)
   {
      if(pdPASS != xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY))
      {
         printf("Core_%d No msg in queue\n", core);
      }
//      printf("Core_%d - waiting msg: %lu - available msg: %lu\n", core, uxQueueMessagesWaiting(xQueue), uxQueueSpacesAvailable(xQueue));

      if (ulReceivedValue == ulExpectedValue)
      {
//         printf("Core_%d - Task receive\n", core);
         led_state = !led_state;
         cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
         ulReceivedValue = 0U;
      }
   }
}
