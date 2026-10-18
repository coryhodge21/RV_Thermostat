/**
 * @file: cli.h
 * @date: 2/25/2026
 * @author: Cory W. Hodge
 * @brief: Process CLI (Command Line Interface) requests from user
 */
 #ifndef _CLI_H_
 #define _CLI_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"      // Often needed for input queuing
#include "freeRTOS_CLI.h"        // The CLI command parser
#include "esp_system.h"
#include "esp_log.h"
#include <string.h>


/******   Globals        */

/**
 * devFlag
 * @brief: Globally accessible flag. Set with CLI
 *          Command 'setDevFlag'. Defined in dev.c.
 */
extern int devFlag; 

extern TaskHandle_t xCliTaskHandle;

/******   Function Declerations     */

 /**             - TASK -
  * cli_task
  * @param[in]: pvParameters - unused optional parameters to pass to task
  * @brief: Task for processing user CLI commands
  */
void cli_task(void *pvParameters);

/**
 * processCLICommand
 * @param[in]: cmdStr - the command string buffer
 * @param[in]: cmdStrLen - length of command string
 * @param[in]: respStr - the response string buffer
 * @param[in]: respStrLen - length of response string
 * @brief: wrapper function for passing command string from 
 *          user input, to free RTOS for processing.
 */
bool processCLICommand(char * cmdStr, size_t cmdStrLen, char * respStr, size_t respStrLen);

/**
 * Register CLI Commands
 */
void cliRegisterCmds();

/*******      - COMMAND DEFINITIONS -     */

/**
 * echo command
 * @brief: echo a string back to user console
 *          good to check if system working.
 */
extern const CLI_Command_Definition_t xEchoCommand;

/**
 * DEV Command
 *  @brief: use for general purpose debugging
 */
extern const CLI_Command_Definition_t xDevCmd;

/** 
 * Set Flag Command
 * @brief: Toggle (Set/Clear) global flag 'devFlag'
 */
extern const CLI_Command_Definition_t xDevSetFlag;

/**
 * returnAsList
 * @brief: Return arguments passed as one list
 *          seperated by new line characters.
 */
extern const CLI_Command_Definition_t xReturnAsSingleList;

/**
 * returnAsMultiList
 * @brief: Return arguments passed as a list
 *          seperated by successive CLI cmd
 *          returns.
 */
extern const CLI_Command_Definition_t xReturnAsMultiList;

#endif // _CLI_H_