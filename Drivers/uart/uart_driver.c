/**
 * 
 */

#include "uart_driver.h"    // header file

#include "esp_log.h"        // for ESP_LOG()

#include "driver/gpio.h"    // for GPIO macros

//#include "gpio_pin_assign.h"    // for GPIO Pin Assignments

#include <string.h> // for memset()

// Buffer sizes
#define MAX_BUFFER_LEN (256)

// PORT DEFINITIONS
#define UART2_PORT UART_NUM_2
#define UART2_BAUD (115200)
#define UART2_QUEUE QUEUE_UART_2

#define UART1_PORT UART_NUM_1
#define UART1_BAUD (115200)
#define UART1_QUEUE QUEUE_UART_1

static const char *TAG = "uart_driver";

/**
 * All Uart Q's Defined in this file : uart_driver.c
 */
QueueHandle_t QUEUE_UART_0;
QueueHandle_t QUEUE_UART_1;
QueueHandle_t QUEUE_UART_2;

// /**
//  * Task Handles
//  */
// TaskHandle_t xUart2TaskHandle = NULL;
// TaskHandle_t xUart1TaskHandle = NULL;

/**
 * Configure Uart
 */
esp_err_t configureUart(uart_port_t uartNum, int baudRate, gpio_num_t txPin, gpio_num_t rxPin)
{
    // Configure parameters of a UART driver
    uart_config_t uart_config = {
        .baud_rate = baudRate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Number of events buffer can hold
    const int eventQueueSize = 20;

    if(uartNum == UART_NUM_0)
    {
        //Install UART driver, and get the queue.
        uart_driver_install(uartNum, RING_BUF_LEN, RING_BUF_LEN, /*event queue size*/eventQueueSize, &QUEUE_UART_0, 0);

        // Configure parameters
        uart_param_config(uartNum, &uart_config);
    
        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(uartNum, txPin, rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        ESP_LOGI("uart_driver", "[Uart_0] Tx:GPIO_1, Rx:GPIO_3");

    }
    else if (uartNum == UART_NUM_1)
    {
        //Install UART driver, and get the queue.
        uart_driver_install(uartNum, RING_BUF_LEN, RING_BUF_LEN, /*event queue size*/eventQueueSize, &QUEUE_UART_1, 0);

        // Configure parameters
        uart_param_config(uartNum, &uart_config);
    
        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(uartNum, /*TX*/txPin, /*RX*/ rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        ESP_LOGI("uart_driver", "[Uart_1] Tx:GPIO_5, Rx:GPIO_18");
    }
    else if (uartNum == UART_NUM_2)
    {
        //Install UART driver, and get the queue.
        uart_driver_install(uartNum, RING_BUF_LEN, RING_BUF_LEN, /*event queue size*/eventQueueSize, &QUEUE_UART_2, 0);

        // Configure parameters
        uart_param_config(uartNum, &uart_config);
    
        //Set UART pins (using UART0 default pins ie no changes.)
        uart_set_pin(uartNum, txPin, rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

        ESP_LOGI("uart_driver", "[Uart_2] Tx:GPIO_17, Rx:GPIO_16");
    }
    else
    {
        // Invalid Entry
        ESP_LOGE("uartConfig", "Invalid Uart Selection:%d", uartNum);

        // Return Failure
        return ESP_FAIL;
    }

    // Return Success
    return ESP_OK;
}

/**
 * Configure Uart with Pattern Detection
 */
esp_err_t configureUartWithPatternDetection(uart_port_t uartNum, int baudRate, char patternChar, int patternReps, gpio_num_t txPin, gpio_num_t rxPin)
{
    // Configure Uart basics
    configureUart(uartNum, baudRate, txPin, rxPin);

    // If patter detection enabled
    if(patternReps > 0)
    {
        //Set uart pattern detect function.
        uart_enable_pattern_det_baud_intr(uartNum, patternChar, patternReps, /* timeout */10, /*pre-idle*/0, /*post-idle*/0);
    
        //Reset the pattern queue length to record at most 10 pattern positions.
        uart_pattern_queue_reset(uartNum, 10);
    }

    // Return Success
    return ESP_OK;
}

/**
 * Uart Communication Loop
 */
esp_err_t uartCommunicationLoop(uart_port_t UART_PORT_NUM, QueueHandle_t UART_EVNT_Q, char * msg, size_t msgLen)
{
    // local storage of current event to process
    uart_event_t event;

    // input stream of bytes from uart port. can be single or multiple bytes
    char * inputStream = (char *) malloc(MAX_BUFFER_LEN);

    // clear buffer
    memset(msg, '\0', sizeof(msg));

    // index of command string
    msgLen = 0;

    // Flag to indicate enter key recived
    bool COMMAND_READY = false;

    // optional spy on data
    bool debug = false;
    
    while(1) 
    {
        //Waiting for UART event.
        if (xQueueReceive(UART_EVNT_Q, (void *)&event, (TickType_t)portMAX_DELAY)) {

            switch (event.type) {

            //Event of UART receiving data
            case UART_DATA:

                // Read available bytes from port
                uart_read_bytes(UART_PORT_NUM, inputStream, event.size, portMAX_DELAY);
                
                // Process bytes
                for(int byte = 0; byte < event.size; byte++)
                {
                    // Display input
                    if(debug == true)
                    {
                        ESP_LOGI(TAG,"Read Bytes:%c", inputStream[byte]);
                    }

                    // compare byte against termination chars \n or \r
                    if(inputStream[byte] == '\n' || inputStream[byte] == '\r' || (msgLen == MAX_BUFFER_LEN))
                    {
                        // terminate input string
                        msg[msgLen] = '\0';

                        // Mark COMMAND_READY to process
                        COMMAND_READY = true;

                        // break from loop
                        break;
                    }
                    
                    // otherwise append char to msg string
                    msg[msgLen] = inputStream[byte];

                    // increment command pointer
                    msgLen++;
                    
                    // Continue parsing input until termination char recieved
                    COMMAND_READY = false;
                    
                }
             
                // If Command is Ready for processing
                if(COMMAND_READY)
                {
                    if(debug == true)
                    {
                        ESP_LOGI(TAG, "Input Command:%s", msg);
                    }
                    
                    // Process Command
                    //processCommand(commandString, commandStringLen, commandResponse, MAX_BUFFER_LEN);
                    return ESP_OK;
                }

                break;
            //Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(UART_PORT_NUM);
                xQueueReset(UART_EVNT_Q);
                break;
            //Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(UART_PORT_NUM);
                xQueueReset(UART_EVNT_Q);
                break;
            //Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGI(TAG, "uart rx break");
                break;
            //Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGI(TAG, "uart parity error");
                break;
            //Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(TAG, "uart frame error");
                break;
            default:
                ESP_LOGI(TAG, "Unknown uart event type: %d", event.type);
                break;
            }
        }
    }

    // free memory
    free(inputStream);
    inputStream = NULL;

    // return complete
    return ESP_OK;
}




// /**
//  * Uart 1 Task 
//  * @breif: Testing to ensure uart 1 works
//  */
// void uart1_test_task(void * pvParameters)
// {
//     // Allocate buffer for parsing
//     char * cmdBuffer = (char *) malloc(MAX_BUFFER_LEN);
//     size_t cmdBuffLen = 0;

//     configureUart(UART1_PORT, UART1_BAUD, UART1_TX_PIN, UART1_RX_PIN);

//     while(1)
//     {
//         // Collect data from uart port up to \n or \r, 
//         //  populate response into devBuffer
//         uartCommunicationLoop(UART1_PORT, UART1_QUEUE, cmdBuffer, cmdBuffLen);

//         // Prove data handling works
//         ESP_LOGI(TAG, "%s", cmdBuffer);        

//         // Sleep Task 10 seconds
//         vTaskDelay(100);
//     }

//     if(xUart1TaskHandle != NULL)
//     {
//         vTaskDelete(xUart1TaskHandle);
//     }
//  }

// /**
//  * Uart 2 Task 
//  * @breif: Testing to ensure uart 2 works
//  */
// void uart2_test_task(void * pvParameters)
// {
//     // Allocate buffer for parsing
//     char * cmdBuffer = (char *) malloc(MAX_BUFFER_LEN);
//     size_t cmdBuffLen = 0;

//     configureUart(UART2_PORT, UART2_BAUD);

//     while(1)
//     {
//         // Collect data from uart port up to \n or \r, 
//         //  populate response into devBuffer
//         uartCommunicationLoop(UART2_PORT, UART2_QUEUE, cmdBuffer, cmdBuffLen);

//         // Prove data handling works
//         ESP_LOGI(TAG, "%s", cmdBuffer);        

//         // Sleep Task 10 seconds
//         vTaskDelay(100);
//     }

//     if(xUart2TaskHandle != NULL)
//     {
//         vTaskDelete(xUart2TaskHandle);
//     }
//  }