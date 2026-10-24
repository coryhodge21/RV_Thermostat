/**
 * @file: uart_driver.h
 * @date: 2/27/2026
 * @author: Cory W. Hodge
 * @breif: driver for uarts
 */
#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include "driver/uart.h"    // for uart macros
#include "driver/gpio.h"    // for gpio macros

#define RING_BUF_LEN    512

extern QueueHandle_t QUEUE_UART_0;
extern QueueHandle_t QUEUE_UART_1;
extern QueueHandle_t QUEUE_UART_2;

extern TaskHandle_t xUart1TaskHandle;
extern TaskHandle_t xUart2TaskHandle;

/**
 * Configure Uart
 */
esp_err_t configureUart(uart_port_t uartNum, int baudRate, gpio_num_t txPin, gpio_num_t rxPin);

/**
 * Configure Uart with Pattern Detection
 */
esp_err_t configureUartWithPatternDetection(uart_port_t uartNum, int baudRate, char patternChar, int patternReps, gpio_num_t txPin, gpio_num_t rxPin);

/**
 * Uart Communication Loop
 * @param[in]:UART_PORT_NUM - The uart port number
 * @param[in]:UART_EVNT_Q - The associated event q
 * @param[in]:msg - The specified uart port number will write response string to msg buffer 
 *                      when \r or \n or MAX_BUF_LEN input char's received
 * @param[in]:msgLen - Length of message in message buffer
 * 
 */
esp_err_t uartCommunicationLoop(uart_port_t UART_PORT_NUM, QueueHandle_t UART_EVNT_Q, char * msg, size_t msgLen);

// /**     Test Tasks   */
// void uart1_test_task(void * pvParameters);
// void uart2_test_task(void * pvParameters);

#endif // _UART_DRIVER_H_