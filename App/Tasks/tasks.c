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

// Task handle for GPIO
TaskHandle_t xGPIOTaskHandle = NULL;

// Uart 1
TaskHandle_t xUart1TaskHandle = NULL;

// dev
TaskHandle_t xDevTaskHandle = NULL;

//startTasks
void startTasks(void)
{

   /**
    *          TASK'S
    * Priorities are defined from 0 up to configMAX_PRIORITIES - 1
    * 0 is lowest priority (assigned to idle task)
    * (configMAX_PRIORITIES - 1) is highest priority
   */

  // PRIORITY : (24) TASK_PRIO_MAX - Highest
  // Create a task for general development (UART 1)
  xTaskCreate(dev_task, "dev_task", 1024 * 4, NULL, TASK_PRIO_MAX, &xDevTaskHandle);


  // PRIORITY : 1 
  // Create task for Uart 1
  //xTaskCreate(uart1_task, "uart1_test_task", 1024 * 4, NULL, 1, NULL);
   

   /**
     * occupies hardware : I2C_NUM_0
   */
   xTaskCreate(lcd_task, "lcd_task", 2048 * 2, NULL, 4, &xLcdTaskHandle);

   /**
    * occupies hardware : UART_NUM_0
   */
   xTaskCreate(cli_task, "cli_task", 2048 * 2, NULL, 4, &xCliTaskHandle);




   // PRIORITY : 10
   // Create a task to handle CLI functionality (UART 0)
   //xTaskCreate(cli_task, "cli_task", 3072, NULL, TASK_PRIO_MAX - 14, &xCliTaskHandle);

   // PRIORITY : 5
   // GPIO
   xTaskCreate(GPIO_task, "GPIO_task", 1024 * 2, NULL, 5, NULL);
   
   // PRIORITY : 4
   // LCD Display
   // Note : Cannot Run LCD task while I2C master bus task is running
   //xTaskCreate(LCD_task, "lcd_task", 2048, NULL, 4, &xLcdTaskHandle);
  //xTaskCreate(i2c_master_task, "i2c_master_task", (1024 * 6), NULL, 4, &xI2CBusTaskHandle);


   // I2C_Master
   // Note : Only Run Master or Slave at once
   //xTaskCreate(i2c_master_task, "i2c_master_task", 1024 * 4, NULL, 4, &xI2CMasterTaskHandle);
   //xTaskCreate(i2c_slave_task, "i2c_slave_task", 1024 * 4, NULL, 4, &xI2CSlaveTaskHandle);

   /**
    * This works. Simple relay functionality
    * data sent to uart 1, are output on uart 2
    * data sent to uart 2, is output on uart 1
    * @note: tasks using uart 1 or uart 2 cannot be used while this is enabled
    */
   // PRIO : 4
   //xTaskCreate(uart1_task_relay, "uart1_task_relay", 3072, NULL, TASK_PRIO_MAX - 20, &xU1U2ST_TaskHandle);
   //xTaskCreate(uart2_task_relay, "uart2_task_relay", 3072, NULL, TASK_PRIO_MAX - 20, &xU2U1ST_TaskHandle);


   // PRIORITY : 1 
   // Create task for UART 2
   //xTaskCreate(uart2_test_task, "uart2_test_task", 1024 * 2, NULL, 1, NULL);

   // PRIORITY : 1 - Lowest 

   // PRIORITY : 0 - Reserved for IDLE Task
   //          void IDLE_TASK();
}