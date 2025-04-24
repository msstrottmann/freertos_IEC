#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Max critical tasks
#define MAX_TASKS 32

// Ban usage of xTaskCreate
#ifdef xTaskCreate
#undef xTaskCreate
#endif
#define xTaskCreate(...) DO_NOT_USE_xTaskCreate_DIRECTLY_USE_xTaskCreateSafe

// auto task registrator
#define xTaskCreateSafe(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) \
    do { \
        BaseType_t result = xTaskCreate((pvTaskCode), (pcName), (usStackDepth), (pvParameters), (uxPriority), (pxCreatedTask)); \
        if (result == pdPASS && (pxCreatedTask) != NULL) { \
            safety_register_task(*(pxCreatedTask)); \
        } \
    } while (0)

#ifdef __cplusplus
extern "C" {
#endif

void safety_register_task(TaskHandle_t task);
void safety_trace_task_switched_in(TaskHandle_t task);
void vSafetyMonitorTask(void *pvParameters);

#ifdef __cplusplus
}
#endif
