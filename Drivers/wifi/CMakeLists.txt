# CmakeLists.txt for the wifi driver
idf_component_register(
    SRCS "wifi.c"
    INCLUDE_DIRS "."
    REQUIRES 
        freertos
        nvs_flash
        esp_wifi esp_netif
        esp_event
    PRIV_REQUIRES 
        freeRTOS_CLI # freeRTOS CLI extension
        
    )