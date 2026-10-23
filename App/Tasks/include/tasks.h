/**
 * @file: tasks.h
 * @brief: place to keep tasks organized
 * @date: 4/26/2026
 * @author: Cory W. Hodge
 */
#ifndef _TASKS_H_
#define _TASKS_H_

// Maximum Assignable Task Priority Level
#define TASK_PRIO_MAX (configMAX_PRIORITIES - 1)

// for ESPLOG
#include "esp_log.h"

/**
 * startTasks
 * @brief: start all tasks that will run on system
 */
void startTasks(void);

/**
 * Program Tasks
 */

 /**         LCD TASK     */
 void lcd_task(void * pvParameters);

/**         CLI TASK     */
void cli_task(void *pvParameters);

/**         gpio_task    */
void GPIO_task(void *pvParameters);

#endif // _TASKS_H_