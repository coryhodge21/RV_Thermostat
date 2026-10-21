/**
 * @file lcdController.h
 * @date 10/20/2026
 * @author Cory W. Hodge
 * @brief control lcd within application using queue system
 */

 #ifndef _LCD_CONTROLLER_
 #define _LCD_CONTROLLER_

 // core librarys
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// max size of message data
#define LCD_CONT_Q_MAX_MSG_DATA_LEN 32

// max number of message slots in Q
#define LCD_CONT_Q_MAX_NUM_MESSAGES 10

// Global Handle for the LCD CONTROLLER Q
extern QueueHandle_t LCD_CONT_Q;

/**
 * Define Message Types
 */
typedef enum {
    LCD_MSG_CMD = 0,
    LCD_MSG_DATA = 1
} lcd_msg_type_t;

/**
 * Define a Message 
 */
typedef struct {
    // message data
    uint8_t data[LCD_CONT_Q_MAX_MSG_DATA_LEN];

    // msg id
    unsigned int msgID;

    // message type
    lcd_msg_type_t msgType;

} lcd_msg_t;

/**
 * Initialize the Q
 */
QueueHandle_t LCDCONTQ_initializeQueue(QueueHandle_t lcdContQ, uint8_t queueSize);

/**
 * Enqueue message
 */
BaseType_t LCDCONTQ_enqueueMessage(QueueHandle_t lcdContQ, lcd_msg_t msg);

/**
 * Dequeue message
 */
void LCDCONTQ_dequeueMessage(QueueHandle_t lcdContQ, lcd_msg_t msg);

#endif // _LCD_CONTROLLER_