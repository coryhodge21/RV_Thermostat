/**
* @file gpio_task.c
* @date 10/21/2026
* @author Cory W. Hodge
* @brief gpio section of application
 */
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"      // Often needed for input queuing

// General Include for unified tasks
#include "tasks.h"

// freeRTOS driver for uart macros
#include "driver/gpio.h"

// for gpio wrappers
#include "gpio_driver.h"

// for project pin layout
#include "gpio_pin_assign.h"

// for ESPLOG
#include "esp_log.h"

/**
 * Global Task Handle
 */
extern TaskHandle_t xGPIOTaskHandle;

/**
 * Global Flag for ISR
 */
bool GPIO_ISR_FLAG = 0;         // general purpose gpio flag
bool BUTTON_1_ISR_FLAG = 0;
bool BUTTON_2_ISR_FLAG = 0;
bool BUTTON_3_ISR_FLAG = 0;


/**         GPIO ISR    */
void IRAM_ATTR button_1_gpio_isr_handler(void* arg) {
    
    // Set a flag when button is pushed to toggle LED blinking
    BUTTON_1_ISR_FLAG = !BUTTON_1_ISR_FLAG;

}

/**         GPIO ISR    */
void IRAM_ATTR button_2_gpio_isr_handler(void* arg) {
    
    // Set a flag when button is pushed to toggle LED blinking
    BUTTON_2_ISR_FLAG = !BUTTON_2_ISR_FLAG;

}

/**         GPIO ISR    */
void IRAM_ATTR button_3_gpio_isr_handler(void* arg) {
    
    // Set a flag when button is pushed to toggle LED blinking
    BUTTON_3_ISR_FLAG = !BUTTON_3_ISR_FLAG;

}

/**
 * gpio_configure_pins(void)
 * @breif configure the pins for the application
 * @return true with no assignment/config issues, false otherwise
 */
bool gpio_configure_pins(void)
{
    //  Only Call This Once : Install the GPIO ISR service (enables per pin interrupts)
    //      ESP_INTR_FLAG_IRAM = requires IRAM_ATTR ISR's to be places in IRAM
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);

    // Declare GPIO I/O Configuration Struct
    gpio_config_t gpioConfig;

     /**     LEDs       */

    /**
     *  Configure LED GPIO  
     * - Output Mode
     * - Pull-Up Disabled (ignored in output mode)
     * - Pull-Up Disable (ignored in output mode)
     * - No interrupt routine
    */       
    gpioConfig.pin_bit_mask = (1ULL << LED_GPIO); 
    gpioConfig.mode = GPIO_MODE_OUTPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpioConfig.intr_type = GPIO_INTR_DISABLE;

    // LED Configure
    gpio_config(&gpioConfig);


    /**     Push Buttons     */

    /**
     * Configure BUTTON 1 GPIO 
     * - Input Mode
     * - Pull-Up Enabled (Default High) 
     * - Pull-Down Disbaled (Default Low)
     *  - FALLIING EDGE INTERRUPT TRIGGERED
    */         
    gpioConfig.pin_bit_mask = (1ULL << BUTTON_1_GPIO); 
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;             // No internal Resistor (pins 34-39) add 10K resistor to pull high
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_NEGEDGE;

    // BUTTON Configure
    gpio_config(&gpioConfig);

    // Add the ISR handler for the specific GPIO pin
    gpio_isr_handler_add(BUTTON_1_GPIO, button_1_gpio_isr_handler, (void*) BUTTON_1_GPIO);

    /**
     * Configure BUTTON 2 GPIO 
     * - Input Mode
     * - Pull-Up Enabled (Default High) 
     * - Pull-Down Disbaled (Default Low)
     *  - FALLIING EDGE INTERRUPT TRIGGERED
    */         
    gpioConfig.pin_bit_mask = (1ULL << BUTTON_2_GPIO); 
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;             // No internal Resistor (pins 34-39) add 10K resistor to pull high
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_NEGEDGE;

    // BUTTON Configure
    gpio_config(&gpioConfig);

    // Add the ISR handler for the specific GPIO pin
    gpio_isr_handler_add(BUTTON_2_GPIO, button_2_gpio_isr_handler, (void*) BUTTON_2_GPIO);

    /**
     * Configure BUTTON 3 GPIO 
     * - Input Mode
     * - Pull-Up Enabled (Default High) 
     * - Pull-Down Disbaled (Default Low)
     *  - FALLIING EDGE INTERRUPT TRIGGERED
    */         
    gpioConfig.pin_bit_mask = (1ULL << BUTTON_3_GPIO); 
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;                 // Does have internal Resistor add 220ohm resistor to pull high for LED
    gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioConfig.intr_type = GPIO_INTR_NEGEDGE;

    // BUTTON Configure
    gpio_config(&gpioConfig);

    // Add the ISR handler for the specific GPIO pin
    gpio_isr_handler_add(BUTTON_3_GPIO, button_3_gpio_isr_handler, (void*) BUTTON_3_GPIO);

    return true;
}

/***        GPIO TASK          */
void GPIO_task(void *arg)
{

    /**
     * Perform GPIO configuration for system/app
     */
    gpio_configure_pins();

    // GPIO Processing Task
    while(1)
    {
        // Button 1
        if(BUTTON_1_ISR_FLAG)
        {
            togglePin(LED_GPIO);
        }
        // Button 2
        if(BUTTON_2_ISR_FLAG)
        {
            togglePin(LED_GPIO);
        }
        // Button 3
        if(BUTTON_3_ISR_FLAG)
        {
            togglePin(LED_GPIO);
        }
                    // Blink LED
           // blink_led(LED_GPIO);
        
        if(BUTTON_1_ISR_FLAG || BUTTON_2_ISR_FLAG || BUTTON_3_ISR_FLAG)
        {

            ESP_LOGI("GPIO_TASk:", "ISR_FLAGs: 1-%d 2-%d 3-%d", BUTTON_1_ISR_FLAG, BUTTON_2_ISR_FLAG, BUTTON_3_ISR_FLAG);
        }

        vTaskDelay(pdMS_TO_TICKS(500));

        //ESP_LOGI("GPIO_TASk:", "ISR_FLAGs: 1-%d 2-%d 3-%d", BUTTON_1_ISR_FLAG, BUTTON_2_ISR_FLAG, BUTTON_3_ISR_FLAG);

//        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
