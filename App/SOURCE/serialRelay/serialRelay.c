/**
 * @file: serialRelay.c
 * @brief: Test area for serial relay functionality
 */

#include "serialRelay.h"
//#include "uart_driver.h"   // for uart function wrapersa
#include "gpio_pin_assign.h"    // for GPIO pin assignments
#include "esp_log.h"        // for ESP_LOG()

size_t MAX_EVNT_CNT = SIZE_MAX;

/**
 * Serial Relay Routine
 */
void serialRelayRoutine(uart_port_t uartTxPort, uart_port_t uartRxPort, QueueHandle_t uartRxEvntQueue, const char * taskName)
{
    uart_event_t event;
    //size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(1024);
    int msgLen = 0;

    size_t evntCnt = 0;

    while(1) {
        
        //Waiting for UART event.
        if (xQueueReceive(uartRxEvntQueue, (void *)&event, (TickType_t)portMAX_DELAY)) {
            //bzero(dtmp, RD_BUF_SIZE);
            switch (event.type) {
            //Event of UART receiving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            case UART_DATA:
                if((msgLen = uart_read_bytes(uartRxPort, dtmp, event.size, portMAX_DELAY)) > 0)
                {
                    if(evntCnt == MAX_EVNT_CNT)
                    {
                        evntCnt = 0;
                    }
                    evntCnt++;

                    uart_write_bytes(uartTxPort, (const char*) dtmp, event.size);

                    // spy on data
                    for(int i = 0; i < msgLen; i++)
                    {
                        ESP_LOGI(taskName, "[e:%zu]%c", evntCnt, dtmp[i]);
                    }
                }
                break;
            //Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(taskName, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(uartRxPort);
                xQueueReset(uartRxEvntQueue);
                break;
            //Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(taskName, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(uartRxPort);
                xQueueReset(uartRxEvntQueue);
                break;
            //Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGI(taskName, "uart rx break");
                break;
            //Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGI(taskName, "uart parity error");
                break;
            //Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(taskName, "uart frame error");
                break;
            //UART_PATTERN_DET
            // case UART_PATTERN_DET:
            //     uart_get_buffered_data_len(uartRxPort, &buffered_size);
            //     int pos = uart_pattern_pop_pos(uartRxPort);
            //     ESP_LOGI(taskName, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
            //     if (pos == -1) {
            //         // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
            //         // record the position. We should set a larger queue size.
            //         // As an example, we directly flush the rx buffer here.
            //         uart_flush_input(uartRxPort);
            //     } else {
            //         uart_read_bytes(uartRxPort, dtmp, pos, 100 / portTICK_PERIOD_MS);
            //         uint8_t pat[PATTERN_CHR_NUM + 1];
            //         memset(pat, 0, sizeof(pat));
            //         uart_read_bytes(uartRxPort, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
            //         ESP_LOGI(taskName, "read data: %s", dtmp);
            //         ESP_LOGI(taskName, "read pat : %s", pat);
            //     }
            //     break;
            //Others
            default:
                ESP_LOGI(taskName, "uart event type: %d", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}


/**
 * Uart1->Uart2 Test Task
 */
TaskHandle_t xU1U2ST_TaskHandle = NULL;
extern QueueHandle_t QUEUE_UART_1;
void uart1_task_relay(void * pvParameters)
{
    // configure uart
    configureUart(UART_NUM_1, 115200, TX_PIN_UART_1, RX_PIN_UART_1);

    // perform operation
    serialRelayRoutine(UART_NUM_2, UART_NUM_1, QUEUE_UART_1, "U1");
}
/**
 * Uart2->Uart1 Test Task
 */
TaskHandle_t xU2U1ST_TaskHandle = NULL;
extern QueueHandle_t QUEUE_UART_2;
void uart2_task_relay(void * pvParameters)
{
    // configure uart
    configureUart(UART_NUM_2, 115200, TX_PIN_UART_2, RX_PIN_UART_2);

    // perform operation
    serialRelayRoutine(UART_NUM_1, UART_NUM_2, QUEUE_UART_2, "U2");
}