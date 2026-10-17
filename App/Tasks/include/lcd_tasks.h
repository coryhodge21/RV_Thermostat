/**
 * @file lcd_tasks.h
 * @date 10/16/2026
 * @author Cory W. Hodge
 * @breif calling module to control LCD with lcd exposed api's
 */

 #ifndef _LCD_TASKS_H_
 #define _LCD_TASKS_H_

 /**
  * lcd_task
  * @brief: Task to control the LCD display using the LCD_I2C library.
  */
void lcd_task(void * pvParameters);

 #endif // _LCD_TASKS_H_