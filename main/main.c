#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_driver.h"
#include "tasks.h"

void app_main(void)
{
    startTasks();
}
