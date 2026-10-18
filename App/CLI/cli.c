/**
 * @file: CLI.c 
 */
#include "cli.h"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "uart_driver.h"    // custom uart wrapper
#include "esp_log.h"
#include "gpio_pin_assign.h"    // for GPIO pin assignments

// Uart Port for CLI console interaction
#define CLI_PORT UART_NUM_0
#define CLI_PORT_BAUD 115200
#define CLI_BUFFER_LEN (256)
#define CLI_PORT_QUEUE QUEUE_UART_0 
extern QueueHandle_t QUEUE_UART_0;

// ESP_LOGx output tag
static const char *TAG = "CLI";

// Global handle, instantiated in main 
TaskHandle_t xCliTaskHandle = NULL;

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

// processCLICommand
bool processCLICommand(char * cmdStr, size_t cmdStrLen, char * respStr, size_t respStrLen)
{
    // Limit to number of output cycles of any command
    const int MAX_CMD_CYCLES = 20;
    int cmdCycles = 0;
    bool echoInput = true;
    
    if(echoInput)
    {
        // Echo input string to console before processing
        ESP_LOGI(TAG, "CMD:%s", cmdStr);
    }

    // if Command returns pdTRUE - there are more lines of output to process
    //  if Command returns pdFalse - there are no more lines of output
    while(FreeRTOS_CLIProcessCommand(cmdStr, respStr, respStrLen) == pdTRUE) 
    {
        // Increment cycle count
        cmdCycles++;

        // Display Command Output
        ESP_LOGI(TAG, "RESP:%s", respStr);

        // if msg outputs more than MAX_CMD_CYCLES cycles consider it stuck in a loop
        if(cmdCycles == MAX_CMD_CYCLES)
        {
            ESP_LOGI(TAG, "Abort: Message Stuck - Exec[%d]", MAX_CMD_CYCLES);
            break;
        }
    }

    // Display Final Command Output
    if(respStrLen > 0)
    {
        ESP_LOGI(TAG, "RESP:%s", respStr);
    }

    // Reset Cmd Cycle Count
    cmdCycles = 0;

    //return success
    return true;
}

/******************************************************************/
/**
 *  CLI COMMANDS
 */
/******************************************************************/

/**
 *  Example : xFunctionName
 * @param[in]: pcWriteBuffer - function should write output to this buffer.
 * @note: pcWriteBuffer - must be null terminated string
 * @param[in]: xWriteBufferLen - length of buffer
 * @param[in]: pcCommandString - A pointer to the entire command string
 * @return: pdFALSE if the generated output is the end of the output, 
 *           meaning there are no more lines to be generated, and the 
 *           command execution is complete
 * @return:  pdTRUE if the returned output is not the end of the output, 
 *             and there are still one or more lines to be generated 
 *             before the command execution is complete.
*/
// static BaseType_t xFunctionName( char *pcWriteBuffer,
//                            size_t xWriteBufferLen,
//                            const char *pcCommandString )
//{
//  ... operate on globals, respond to user input, ... 
//}

/**************************************************************************** */

/**
 * echo
 * 
 */
static BaseType_t prvEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;

    // Get the first parameter (everything after "echo ")
    //  this function used ' ' spaces as delimitation between input strings
    pcParameter = FreeRTOS_CLIGetParameter(
                        pcCommandString,        // The command string itself.
                        1,                      // The index of the parameter (1 for the first).
                        &xParameterStringLength // Out parameter: receives length of the parameter.
                    );

    // Check if a parameter was found
    if( pcParameter != NULL )
    {
        // Copy the parameter into the output buffer
        snprintf( pcWriteBuffer, xWriteBufferLen, "Echoing: %.*s\r\n", xParameterStringLength, pcParameter );
    }
    // ! CLI Cmd Def defined with exactly 1 parameter
    // !  this means echo called with 0 or >1 parameters will not be processed
    // else
    // {
    //     // If no parameter, print a usage message
    //     snprintf( pcWriteBuffer, xWriteBufferLen, "Usage: echo <message>\r\n" );
    // }
    
    // pdFALSE = No more output to process from this command
    return pdFALSE; 
}

// Command Definition
const CLI_Command_Definition_t xEchoCommand =
{
    "echo",                                    /* The command string. */
    "echo <message>: Echoes the message back to the console.\r\n", /* Help string. */
    prvEchoCommand,                            /* The callback function. */
    1                                          /* One parameter is expected. */
};

/**************************************************************************** */

/**
 * prvDevSetFlag
 * 
 */
static BaseType_t prvDevSetFlag( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    // devFlag defined in dev.c
    // Toggle the value of this flag 
    // when this command is issued
    if(devFlag == 0)
    {
        devFlag = 1;
    } else 
    {
        devFlag = 0;
    }
    
    // Build Output String and write to output buffer
    snprintf(pcWriteBuffer, xWriteBufferLen, "DevFlag:%d", devFlag);
    
    // Return End of Process Signal
    return pdFALSE;
}

// Command Definition
const CLI_Command_Definition_t xDevSetFlag =
{
    "devSetFlag",                                    /* The command string. */
    "devSetFlag: Sets a Flag in code to trigger an event.\r\n", /* Help string. */
    prvDevSetFlag,                            /* The callback function. */
    0                                          /* No parameter is expected. */
};

/**************************************************************************** */

/**
 * dev
 * DEV Command
 *  - use for general purpose debugging
 */
static BaseType_t prvDevCmd( char *cmdOutputBuffer, size_t cmdOutBufSize, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterLength = 0; 
    BaseType_t xParameterNumber = 1;

    // Start with a clean buffer
    memset(cmdOutputBuffer, '\0', cmdOutBufSize);

    // Loop to parse variable parameters
    do {
        pcParameter = FreeRTOS_CLIGetParameter(
                        pcCommandString,
                        xParameterNumber,
                        &xParameterLength );

        if( pcParameter != NULL ) {
            
            /**
             * Process Parameter
             */
            ESP_LOGI("DEV", "Passed Param: %s", pcParameter); 

            xParameterNumber++;
        }
    } while( pcParameter != NULL );

    return pdFALSE; // Command finished
}

// Command Definition
const CLI_Command_Definition_t xDevCmd =
{
    "dev",                                      /* The command string. */
    "dev <param1> <param2>: args space delimited\r\n", /* Help string. */
    prvDevCmd,                                  /* The callback function. */
    -1                                          /* variable number of parameters. */
};

/**************************************************************************** */

/**
 * Return As Single List
 * @brief: Returns parameters as one '\n' seperated list 
 */
static BaseType_t prvReturnAsSingleList( char *cmdOutputBuffer, size_t cmdOutBufSize, const char *pcCommandString )
{
    // Local storage of input parameter
    const char *pcParameter = {0};

    // Individual Parameters parsed up to space
    char parsedParameter[CLI_BUFFER_LEN] = {0};
    
    // Returned : length of input parameter being parsed
    BaseType_t xParameterStringLength;
    
    // paramter count starts at 1
    static int parameterCount = 1;

    // Clear output Buffer
    memset(cmdOutputBuffer, '\0', cmdOutBufSize);

    do 
    {
        // Get parameter : This function uses (' ') spaces as 
        //  delimitation between input parameters. and input is
        //  terminated when '\n' (or ?'\r') recieved
        pcParameter = FreeRTOS_CLIGetParameter(
                        pcCommandString,        // The input command string.
                        parameterCount,         // The index of the parameter.
                        &xParameterStringLength // Out parameter: receives length of the parameter.
                    );

        // if parameters exist
        if(pcParameter != NULL)
        {
            // Clear out parsing buffer
            memset(parsedParameter, '\0', CLI_BUFFER_LEN);
        
            // Copy parameter out of string 
            strncpy(parsedParameter, pcParameter, xParameterStringLength);

            // Copy parameter into output buffer, Build up output buffer 
            //  calculate starting point of writing: head of buffer + current length of string
            //  calculate remaining length of available buffer : Total size - current size
            snprintf((cmdOutputBuffer + strlen(cmdOutputBuffer)), (cmdOutBufSize - strlen(cmdOutputBuffer)), \
                "[param:%d](%d):%s\n", parameterCount, xParameterStringLength, parsedParameter);

            // Increase Parameter counter for next iteration
            parameterCount++;
        }

    } while( pcParameter != NULL );
    
    // End of Command
    return pdFALSE; 
}

// Command Definition
const CLI_Command_Definition_t xReturnAsSingleList =
{
    "returnAsSingleList",                                    /* The command string. */
    "returnAsSingleList <param1> <param2>: Return each parameter in one, \\n seperated list from CLI command\r\n", /* Help string. */
    prvReturnAsSingleList,                                  /* The callback function. */
    -1                                              /* variable number of parameters. */
};

/**************************************************************************** */

/**
 * xReturnAsMultiList
 * @brief: Return parameters individually from CLI command until list complete
 */
static BaseType_t prvReturnAsMultiList( char *cmdOutputBeffer, size_t cmdOutBufSize, const char *pcCommandString )
{
    // Local storage of input parameter
    const char *pcParameter = {0};

    // Individual Parameters parsed up to space
    char parsedParameter[CLI_BUFFER_LEN] = {0};
    
    // Returned : length of input parameter
    BaseType_t xParameterStringLength;
    
    // paramter count must start at 1
    static int parameterCount = 1;

    
    // Get parameter : This function uses (' ') spaces as delimitation between input pareters
    pcParameter = FreeRTOS_CLIGetParameter(
                    pcCommandString,        // The input command string.
                    parameterCount,                      // The index of the parameter.
                    &xParameterStringLength // Out parameter: receives length of the parameter.
                );


    // if parameter exists
    if( pcParameter != NULL)
    {
        // DEBUG
        //ESP_LOGI("RAML", "[pCnt:%d](pLen:%d)p:%s", parameterCount, xParameterStringLength, pcParameter); 

        // Copy parameter out of string 
        strncpy(parsedParameter, pcParameter, xParameterStringLength);

        // Copy parameter into output buffer
        snprintf(cmdOutputBeffer, cmdOutBufSize, "[param:%d](%d):%s", parameterCount, xParameterStringLength, parsedParameter);

        // Increment parameter count
        parameterCount++;

        // return : Command still processing
        return pdTRUE;
    }

    // clear parameter counter
    parameterCount = 0;

    // Copy end of transmission into output buffer
    snprintf(cmdOutputBeffer, cmdOutBufSize, "-End of Output-");

    // End of Command
    return pdFALSE; 
}

// Command Definition
 const CLI_Command_Definition_t xReturnAsMultiList =
{
    "returnAsMultiList",                                    /* The command string. */
    "returnAsMultiList <param1> <param2>: Return each parameter individually from CLI command\r\n", /* Help string. */
    prvReturnAsMultiList,                                  /* The callback function. */
    -1                                              /* variable number of parameters. */
};

/**************************************************************************** */

/**
 * toggleGpioIsrFlag
 *  - toggle flag normally toggled by gpio isr
 */
static BaseType_t prvToggleGpioIsrFlag( char *cmdOutputBuffer, size_t cmdOutBufSize, const char *pcCommandString )
{
    // Set a flag when button is pushed to toggle LED blinking
    GPIO_ISR_FLAG = !GPIO_ISR_FLAG;
    
    // Start with a clean buffer
    memset(cmdOutputBuffer, '\0', cmdOutBufSize);

    // Write Message to Console
    snprintf(cmdOutputBuffer, cmdOutBufSize, "GPIO_ISR_FLAG:%d", GPIO_ISR_FLAG);

    return pdFALSE; // Command finished
}

// Command Definition
const CLI_Command_Definition_t xToggleGpioIsrFlag =
{
    "toggleGpioIsrFlag",                                        /* The command string. */
    "toggleGpioIsrFlag: toggles flag normally toggled by gpio isr routine\r\n", /* Help string. */
    prvToggleGpioIsrFlag,                                       /* The callback function. */
    0                                                           /* number of parameters. */
};

/**************************************************************************** */


/******************************************************************/
/**
 *  REGISTER CLI COMMANDS
 */
/******************************************************************/
void cliRegisterCmds()
{
        // cli.h commands
    FreeRTOS_CLIRegisterCommand( &xEchoCommand );

    FreeRTOS_CLIRegisterCommand( &xDevCmd );
    
    FreeRTOS_CLIRegisterCommand( &xDevSetFlag );

    FreeRTOS_CLIRegisterCommand( &xReturnAsSingleList );

    FreeRTOS_CLIRegisterCommand( &xReturnAsMultiList );

    FreeRTOS_CLIRegisterCommand( &xToggleGpioIsrFlag );
}
/**
 *  END OF FILE : Keep cliRegisterCmds() as last function in file
 *    (after CLI commands are defined above) 
 */