/** 
 * @file tasks.c
 * @date: 4/26/2026
 * @author: Cory W. Hodge
 * @brief: place to keep tasks organized
 */

// for xTaskCreate() and TaskHandle_t
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// for tasks includes
#include "tasks.h"

// Define Task Handles
TaskHandle_t xLcdTaskHandle = NULL;

// Global handle for CLI task
TaskHandle_t xCliTaskHandle = NULL;

//startTasks
void startTasks(void)
{
    xTaskCreate(lcd_task, "lcd_task", 2048, NULL, 4, &xLcdTaskHandle);

    xTaskCreate(cli_task, "cli_task", 2048, NULL, 4, &xCliTaskHandle);
}