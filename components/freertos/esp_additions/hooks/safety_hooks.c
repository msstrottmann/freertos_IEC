#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define MAX_TASKS 32

static TaskHandle_t monitored_tasks[MAX_TASKS];
static volatile uint8_t task_flags[MAX_TASKS];
static int task_count = 0;

void safety_register_task(TaskHandle_t task) {
    if (task_count < MAX_TASKS) {
        monitored_tasks[task_count] = task;
        task_flags[task_count] = 0;
        task_count++;
    }
}

void safety_trace_task_switched_in(TaskHandle_t task) {
    for (int i = 0; i < task_count; i++) {
        if (task == monitored_tasks[i]) {
            task_flags[i] = 1;
            break;
        }
    }
}

void vSafetyMonitorTask(void *pvParameters) {
    while (1) {
        for (int i = 0; i < task_count; i++) {
            if (task_flags[i] == 0) {
                ESP_LOGE("SAFETY", "Critical Task not Replying. Reboot...");
                esp_restart();  // reboot in safe mode
            }
            task_flags[i] = 0; // Reset for next cycle 
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // watchdog interval
    }
}
