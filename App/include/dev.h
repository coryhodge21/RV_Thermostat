/**
 * @file: dev.h
 * @date: 2/26/2026
 * @author: Cory W. Hodge
 * @brief:  file for general development and sandbox expirementing
 */
#ifndef _DEV_H_
#define _DEV_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"      // Often needed for input queuing
#include "esp_system.h"
#include "esp_log.h"
#include <string.h>

// Link Dev Flag to this module
extern int devFlag;

extern TaskHandle_t xDevTaskHandle;

/**
 * dev_task
 * 
 */
void dev_task(void * pvParameters);

/**
 * 
 */

#endif // _DEV_H_