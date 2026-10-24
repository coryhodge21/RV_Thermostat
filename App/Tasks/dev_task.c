

#define UART1_PORT UART_NUM_1
#define UART1_BAUD (115200)
#define UART1_QUEUE QUEUE_UART_1

// Buffer sizes
#define MAX_BUFFER_LEN (256)

static const char *TAG = "DEV >>";

// for dev header
#include "tasks.h"

// for FreeRTOS library
#include "freertos/FreeRTOS.h"  // xQueueCreate

#include "uart_driver.h"    // for uart wrapper functions

#include "gpio_pin_assign.h" // for pin assignments

#include "esp_log.h"        // for ESP_LOG()

QueueHandle_t QUEUE_UART_0 = NULL;
QueueHandle_t QUEUE_UART_1 = NULL;
QueueHandle_t QUEUE_UART_2 = NULL;

// toggle output to relay to uart 0/main output
bool RELAY_UART_0 = true;

/**
 * Uart 1 Task 
 * @breif: Testing to ensure uart 1 works
 */
void dev_task(void * pvParameters)
{
    // Allocate buffer for parsing
    //char * cmdBuffer = (char *) malloc(MAX_BUFFER_LEN);
    char cmdBuffer[MAX_BUFFER_LEN]={0};
    size_t cmdBuffLen = 0;

    // Configure uart
    configureUart(UART1_PORT, UART1_BAUD, TX_PIN_UART_1, RX_PIN_UART_1);

    
    if(QUEUE_UART_1 == NULL)
    {
        // output dev msg and abort due to memory budget
        ESP_LOGE(TAG, "queue_uart_1 == NULL");
        abort();   
    }

    while(1)
    {
        // Collect data from uart port up to \n or \r, 
        //  populate response into devBuffer
        uartCommunicationLoop(UART1_PORT, UART1_QUEUE, cmdBuffer, cmdBuffLen);

        // uart 0/default output port
        if(RELAY_UART_0 == true)
        {
            // Prove data handling works
            ESP_LOGI(TAG, "%s", cmdBuffer);        
        }

        // Sleep Task 100 ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    if(xUart1TaskHandle != NULL)
    {
        vTaskDelete(xUart1TaskHandle);
    }
 }