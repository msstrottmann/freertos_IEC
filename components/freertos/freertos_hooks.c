#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"  // Para esp_restart()

// call hook when FreeRTOS detects overflow
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // Error message
    printf("ERROR: Stack Overflow in Task: %s\n", pcTaskName);

    //Action Restart
    esp_restart();
}
