
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"      // Often needed for input queuing
#include "freeRTOS_CLI.h"        // The CLI command parser
#include "esp_system.h"
#include "esp_log.h"
#include <string.h>

#include "cli.h"
#include "uart_driver.h"    // custom uart wrapper
#include "gpio_pin_assign.h"    // for GPIO pin assignments

// Uart Port for CLI console interaction
#define CLI_PORT UART_NUM_0
#define CLI_PORT_BAUD 115200
#define CLI_BUFFER_LEN (256)
#define CLI_PORT_QUEUE QUEUE_UART_0 
extern QueueHandle_t QUEUE_UART_0;

// Global handle for CLI task
extern TaskHandle_t xCliTaskHandle;

/**
 * GPIO_ISR_FLAG
 * @breif: Flag set with cli cmd: toggleGpioIsrFlag
 *          This flag is normally operated by the 
 *          gpio ISR triggered by pushbutton event.
 */
extern bool GPIO_ISR_FLAG;


/******************************************************************/
/**
 *  CLI TASK
 */
/******************************************************************/

/**         CLI TASK     */
void cli_task(void *pvParameters)
{
    // Allocate buffer for parsing
    char * cliBuffer = (char *) malloc(CLI_BUFFER_LEN);
    char * respBuffer = (char *) malloc(CLI_BUFFER_LEN);

    // -------------------------------
    // Configure Uart
    // ------------------------------
    configureUart(CLI_PORT, CLI_PORT_BAUD, TX_PIN_UART_0, RX_PIN_UART_0);

    // ------------------------------
    // Register CLI Commands
    // ------------------------------
    cliRegisterCmds();

    //  --------------------------------------
    //      TASK LOOP
    //  --------------------------------------
    while(1)
    {
        // Collect data from uart port up to \n or \r, 
        //  populate response into Buffer
        uartCommunicationLoop(CLI_PORT, CLI_PORT_QUEUE, cliBuffer, CLI_BUFFER_LEN);
    
        // pass returned string into processing routine
        processCLICommand(cliBuffer, CLI_BUFFER_LEN, respBuffer, CLI_BUFFER_LEN);
    }

    // free cli buffer
    free(cliBuffer);
    cliBuffer = NULL;

    // free resp buffer
    free(respBuffer);
    respBuffer = NULL;

    // Free Task
    if(xCliTaskHandle != NULL)
    {
        vTaskDelete(xCliTaskHandle);
    }
}