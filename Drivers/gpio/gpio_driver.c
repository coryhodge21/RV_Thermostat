/**
 * gpio_driver.c
 * @note : See cli.c for gpio cli command - toggleGpioIsrFlag
 * 
 */


 /*     Includes    */
#include "freertos/FreeRTOS.h"  // for IRAM_ATTR
#include "gpio_driver.h"        // Project Driver
#include <string.h>             // for memset()
//#include "esp_log.h"          // for ESP_LOG
//#include "freertos/task.h"  // for TaskHandle_t
//#include "driver/gpio.h"    // freeRTOS driver for uart macros

// /**
//  * Global Task Handle
//  */
// TaskHandle_t xGPIOTaskHandle = NULL;

// /**
//  * Global Flag for ISR
//  */
// bool GPIO_ISR_FLAG = 0;


// /**         GPIO ISR    */
// static void IRAM_ATTR gpio_isr_handler(void* arg) {
    
//     // Set a flag when button is pushed to toggle LED blinking
//     GPIO_ISR_FLAG = !GPIO_ISR_FLAG;
// }

// /***        GPIO TASK          */
// void GPIO_task(void *arg)
// {
//     // Declare GPIO I/O Configuration Struct
//     gpio_config_t gpioConfig;

//     /**
//      *  Configure LED GPIO  
//      * - Output Mode
//      * - Pull-Up Disabled (ignored in output mode)
//      * - Pull-Up Disable (ignored in output mode)
//      * - No interrupt routine
//      */       
//     gpioConfig.pin_bit_mask = (1ULL << LED_GPIO); 
//     gpioConfig.mode = GPIO_MODE_OUTPUT;
//     gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
//     gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
//     gpioConfig.intr_type = GPIO_INTR_DISABLE;

//     // LED Configure
//     gpio_config(&gpioConfig);

//     /**
//      * Configure BUTTON GPIO 
//      * - Input Mode
//      * - Pull-Up Enabled (Default High) 
//      * - Pull-Down Disbaled (Default Low)
//      *  - FALLIING EDGE INTERRUPT TRIGGERED
//      */         
//     gpioConfig.pin_bit_mask = (1ULL << BUTTON_GPIO); 
//     gpioConfig.mode = GPIO_MODE_INPUT;
//     gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
//     gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     gpioConfig.intr_type = GPIO_INTR_NEGEDGE;

//     // 1. BUTTON Configure
//     gpio_config(&gpioConfig);

//     // 2. Install the GPIO ISR service (enables per pin interrupts)
//     //      ESP_INTR_FLAG_IRAM = requires IRAM_ATTR ISR's to be places in IRAM
//     gpio_install_isr_service(ESP_INTR_FLAG_IRAM);

//     // 3. Add the ISR handler for the specific GPIO pin
//     gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, (void*) BUTTON_GPIO);

//     while(1)
//     {
//         if(GPIO_ISR_FLAG)
//         {
//             // Blink LED
//             blink_led(LED_GPIO);
//         }

//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }


// Blink GPIO
void blink_led(gpio_num_t led_pin)
{
    gpio_set_level(led_pin, HIGH);
    
    vTaskDelay(pdMS_TO_TICKS(500));

    gpio_set_level(led_pin, LOW);
    
    vTaskDelay(pdMS_TO_TICKS(500));

}