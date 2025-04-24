#ifndef SAFETY_WRAPPERS_H
#define SAFETY_WRAPPERS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "safety_hooks.h"  

#define xTaskCreateSafe(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask) \
    do { \
        BaseType_t result = xTaskCreate((pvTaskCode), (pcName), (usStackDepth), (pvParameters), (uxPriority), (pxCreatedTask)); \
        if (result == pdPASS && (pxCreatedTask) != NULL) { \
            safety_register_task(*(pxCreatedTask)); \
        } \
    } while (0)

#endif // SAFETY_WRAPPERS_H
