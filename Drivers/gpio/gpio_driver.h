/**
 * @file: gpio_driver.h
 * @date: 2/28/2026
 * @author: Cory W. Hodge
 * @breif: driver for gpio functionality
 */
#ifndef _GPIO_DRIVER_H_
#define _GPIO_DRIVER_H_

#include "freertos/task.h"  // for TaskHandle_t
#include "driver/gpio.h"    // freeRTOS driver for uart macros
//#include freertos_ext/FreeRTOS_CLI.h" // for CLI_Command_Definition_t
//#include "gpio_pin_assign.h"

/*  GPIOs   */
#define HIGH          1
#define LOW           0


/**
 * Global Task Handle
 */
extern TaskHandle_t xGPIOTaskHandle;

/**
 * Global ISR Flag
 */
extern bool GPIO_ISR_FLAG;

/**         GPIO ISR    */
void IRAM_ATTR gpio_isr_handler(void* arg);

/**     Function Declerations    */

/**
 * 
 */
//void GPIO_task(void *arg);

/**
 * 
 */
void blink_led(gpio_num_t led_pin);

/**
 * GPIO COMMANDS
 */

 /**
  * toggleGpioIsrFlag
  * @brief: Toggles the flag that is normally
  *           toggled with gpio interrupt
  */
//extern const CLI_Command_Definition_t xToggleGpioIsrFlag;


#endif // _UART_DRIVER_H_