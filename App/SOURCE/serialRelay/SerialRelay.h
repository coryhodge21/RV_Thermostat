/**
 * 
 */

 #ifndef _SERIAL_RELAY_H
 #define _SERIAL_RELAY_H

#include "uart_driver.h"   // for uart function wrapersa

void serialRelayRoutine(uart_port_t uartTxPort, uart_port_t uartRxPort, QueueHandle_t uartRxEvntQueue, const char * taskName);

extern TaskHandle_t xU1U2ST_TaskHandle;
extern QueueHandle_t QUEUE_UART_1;
void uart1_task_relay(void * pvParameters);

extern TaskHandle_t xU2U1ST_TaskHandle;
extern QueueHandle_t QUEUE_UART_2;
void uart2_task_relay(void * pvParameters);

 #endif // _SERIAL_RELAY_H