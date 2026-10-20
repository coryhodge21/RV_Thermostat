/**
 * @file main.c
 * @date: 4/26/2026
 * @author: Cory W. Hodge
 * @brief: Wrapper entry point, to adhere to standark CMake and ESP-IDF project structure.
 */

#include "esp_log.h"
// Project/App/include/app.h
#include "app.h"
// 
#include "git_version.h" // GIT_HASH
extern const char *kGitHash;

// app_main
void app_main(void)
{
    ESP_LOGI("GIT_HASH", "GIT_HASH:%s", kGitHash);

    // Call the application entry point
    application_entry();
}
