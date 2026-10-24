/**
 * 
 */

#include "dev.h"

#include "uart_driver.h"

#include "gpio_pin_assign.h"    // for GPIO pin assignments

#include <stdarg.h> // for va_list

static const char *TAG = "DEV";
/**
 * CLI Command : devSetFlag
 *      this command toggles the value of this flag
 *      intended for general purpose debugging
 *      and expirementing
 */
int devFlag = 0;

TaskHandle_t xDevTaskHandle = NULL;

#define DEV_PORT UART_NUM_1
#define DEV_PORT_BAUD 115200
extern QueueHandle_t QUEUE_UART_1;
#define DEV_PORT_QUEUE QUEUE_UART_1

#define DEV_BUFF_MAX_LEN 1024


/**
 * devOut
 * output string to uart 1
 */
void devOut(const char* msg, ... )
{
    char outStr[128];

    // parse args from va_list
    va_list va_args;
    va_start(va_args, msg);

    // format output string
    int len = vsnprintf(outStr, sizeof(outStr), msg, va_args);

    // end arg parse
    va_end(va_args);

    // check if data was sent
    if(len >= 0)
    {
        // output the msg to the dev buffer
        uart_write_bytes(UART_DEV_PORT, outStr, sizeof(outStr));
    }
    else
    {
        ESP_LOGE("DEV", "ERROR OUTPUT TO DEV PORT");
    }
}


/** DEV TASK    */
void dev_task(void * pvParameters)
{
    // Allocate buffer for parsing
    char * devBuffer = (char *) malloc(DEV_BUFF_MAX_LEN);
    size_t msgLen = 0;

    configureUart(DEV_PORT, DEV_PORT_BAUD, TX_PIN_UART_1, RX_PIN_UART_1);

    while(1)
    {
        // If setDevFlag CLI Command Issued
        if(devFlag == 1)
        {
            ESP_LOGI(TAG, "Dev Flag Set!");
        }

        // Collect data from uart port up to \n or \r, 
        //  populate response into devBuffer
        uartCommunicationLoop(DEV_PORT, DEV_PORT_QUEUE, devBuffer, msgLen);

        // Prove data handling works
        ESP_LOGI(TAG, "%s", devBuffer);        



        // Sleep Task
        vTaskDelay(500);
    }

    if(xDevTaskHandle != NULL)
    {
        vTaskDelete(xDevTaskHandle);
    }
 }