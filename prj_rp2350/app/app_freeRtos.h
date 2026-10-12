/**
 * @file    app_freeRtos.h
 * @author  Jeremie Gallee (jeremie.gallee@noser.com)
 * @brief   FreeRTOS application for project prj_rp2350
 *          Creates some tasks and have fun
 * @version 0.1
 * @date    2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef APP_FREERTOS_H
#define APP_FREERTOS_H

#include "board.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Number of items queue can hold */
#define mainQUEUE_LENGTH                  (1)

#define mainQUEUE_RECEIVE_TASK_PRIORITY   (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY      (tskIDLE_PRIORITY + 1)
#define mainQUEUE_SEND_FREQUENCY_MS       (1 / portTICK_PERIOD_MS)

#define TASK_AFFINITY_CORE_0              (1 << 0)
#define TASK_AFFINITY_CORE_1              (1 << 1)

/**
 * @brief  Init and start FreeRTOS
 */
void vLaunch(void);

#endif /* APP_FREERTOS_H */
