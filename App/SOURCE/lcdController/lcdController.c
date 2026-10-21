/**
 * @file: lcdController.c
 * @date    10/20/2026
 * @author  Cory W. Hodge
 * @ Implementation for LCD Controller Q based system
 */

// Header
#include "lcdController.h"

// for ESPLOG
#include "esp_log.h"

/**
 * GLOBAL : Handle for the LCD CONTROLLER Q
 */
QueueHandle_t LCD_CONT_Q = NULL;

/**
 * Initialize the Q
 */
QueueHandle_t LCDCONTQ_initializeQueue(QueueHandle_t lcdContQ, uint8_t queueSize)
{
    // Create Message Q
    lcdContQ = xQueueCreate(LCD_CONT_Q_MAX_NUM_MESSAGES, sizeof(lcd_msg_t));

    // Check for allocation success
    if (lcdContQ == NULL) {
        ESP_LOGE("lcdController::initializeControllerQ:", "Error creating the queue, pointer = NULL");
        return NULL;
    }

    // return success
    return lcdContQ;
}

/**
 * Enqueue message
 * Send message to queue (blocks for up to 100 ticks if queue is full)
 */
BaseType_t LCDCONTQ_enqueueMessage(QueueHandle_t lcdContQ, lcd_msg_t msg)
{
    if (xQueueSend(lcdContQ, &msg, pdMS_TO_TICKS(100)) == pdTRUE) {
        ESP_LOGI("lcdController::enqueueMessage:", "Sent message ID: %d", msg.msgID);
        return pdTRUE;
    } else {
        ESP_LOGE("lcdController::enqueueMessage:", "Failed to send message: Queue Full");
        return pdFALSE;
    }
}

/**
 * Dequeue message
 * Receive message 
 * Blocks indefinitely until a message arrives
 */
void LCDCONTQ_dequeueMessage(QueueHandle_t lcdContQ, lcd_msg_t msg)
{
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);

    // Recieve message -- BLOCKING
    if (xQueueReceive(lcdContQ, &msg, xTicksToWait) == pdPASS) {
        ESP_LOGI("lcdController::dequeueMessage:", "Received message ID: %d", msg.msgID);
        //return pdTRUE;
    } else {
        // Timeout reached; no item was available
        printf("Queue receive timeout!\n");
        abort();
    }
}