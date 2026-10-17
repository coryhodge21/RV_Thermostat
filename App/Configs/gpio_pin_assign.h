/**
 * @file: gpio_pin_assign.h
 * @author: Cory W. Hodge
 * @date: 3/12/2026
 * @brief: Common file to be shared across project to 
 *          synchronize and localize gpio pin assignments
 * @note: list file thats that rely on pin assingmnets
 */

 /**
  * uart_driver pins
  */

// UART 0
#define TX_PIN_UART_0   UART_PIN_NO_CHANGE      // Default : GPIO_NUM_1
#define RX_PIN_UART_0   UART_PIN_NO_CHANGE      // Default : GPIO_NUM_3
// UART 1
#define TX_PIN_UART_1   GPIO_NUM_5              // Default not recomended : GPIO_NUM_10
#define RX_PIN_UART_1   GPIO_NUM_18             // Default not recomended : GPIO_NUM_9
// UART 2
#define TX_PIN_UART_2   GPIO_NUM_17              // Default : GPIO_NUM_17
#define RX_PIN_UART_2   GPIO_NUM_16             // Default : GPIO_NUM_16

/**
 * GPIO Pins
 */
#define BUTTON_GPIO   GPIO_NUM_35
#define LED_GPIO      GPIO_NUM_25

/**
 * LCD Pins
 */
#define LCD_I2C_SDA     GPIO_NUM_21
#define LCD_I2C_SCL     GPIO_NUM_22

/**
 * I2C Bus Pins
 */
#define I2C_MASTER_SCL    GPIO_NUM_22
#define I2C_MASTER_SDA    GPIO_NUM_21
#define I2C_SLAVE_SCL    GPIO_NUM_22
#define I2C_SLAVE_SDA    GPIO_NUM_21