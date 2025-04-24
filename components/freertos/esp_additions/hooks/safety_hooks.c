#include "esp_additions/hooks/safety_hooks.h"
#include "esp_system.h" // For esp_restart()

static TaskHandle_t monitored_tasks[MAX_TASKS];
static volatile uint8_t task_flags[MAX_TASKS];
static int task_count = 0;

// Critical task registration
void safety_register_task(TaskHandle_t task) {
    if (task_count < MAX_TASKS) {
        monitored_tasks[task_count] = task;
        task_flags[task_count] = 0;
        task_count++;
    } else {
        ESP_LOGW("SAFETY", "TASK LIMIT EXCEED (MAX_TASKS reached)");
    }
}

// Task Activation Detection Hook
void safety_trace_task_switched_in(TaskHandle_t task) {
    for (int i = 0; i < task_count; i++) {
        if (task == monitored_tasks[i]) {
            task_flags[i] = 1;
            break;
        }
    }
}

// restart critical task if no respose
void vSafetyMonitorTask(void *pvParameters) {
    while (1) {
        for (int i = 0; i < task_count; i++) {
            if (task_flags[i] == 0) {
                ESP_LOGE("SAFETY", "Critical Task not Replying. Reboot...");
                esp_restart();  // reboot in safe mode
            }
            task_flags[i] = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  //watchdog interval
    }
}
