# FreeRTOS SIL1 for ESP32 (Based on ESP-IDF)

⚠️ **WARNING: This project is under active development and is not yet certified or guaranteed to fully meet IEC 61508 SIL1 compliance. Use at your own risk in critical environments.**

This repository is a modified version of the FreeRTOS component from [Espressif's ESP-IDF](https://github.com/espressif/esp-idf), tailored to support development of industrial applications requiring safety mechanisms aligned with the IEC 61508 standard (Safety Integrity Level 1 - SIL1).

---

## 🔍 Purpose

To integrate lightweight safety mechanisms into the FreeRTOS kernel for ESP32/ESP32-S3 targets, enabling the detection and handling of critical task failures in real-time systems.

---

## ✅ Key Modifications from Original FreeRTOS

- 📌 **Stack Overflow Protection** enabled:
  - `configCHECK_FOR_STACK_OVERFLOW` set to `2` in `FreeRTOSConfig.h`.

- 🧠 **Custom Scheduler Hook:**
  - Implemented `traceTASK_SWITCHED_IN()` to monitor task context switches and auto-report task activity.

- 🛡️ **Safety Monitor Task:**
  - Added a new high-priority task that checks the liveness of all registered critical tasks and triggers a restart if any fail to respond.

- 📦 **Safety Module Integration:**
  - New file: `esp_additions/hooks/safety_hooks.c`
  - Includes `safety_register_task()` and `vSafetyMonitorTask()`.

- 🏷️ **All tasks treated as critical:**
  - A task must be explicitly registered with the monitor system.

---

## 📈 Planned Features

-    **Critical Task Auto-Registration:**
  - Wrap `xTaskCreate` in a macro to enforce critical registration by default.

-    **Configurable Recovery Policy:**
  - Add support for fallback modes via E-FUSE instead of only restarting.

-    **Runtime Task Diagnostics:**
  - Report CPU load and uptime per task to console.

-    **Memory Safety Enhancements:**
  - Integration with ESP32 hardware watchdog.
  - Detection of memory corruption.
  - automated checks for MISRA-C compliance

-------------------------------------------------------------------------------------------------

## 🧰 Getting Started

Clone the repo and add the `freertos` folder into your ESP-IDF project’s `components/` directory.

## 🧪 Usage

```c
void app_main(void) {
    TaskHandle_t taskHandle;

    xTaskCreate(vTaskA, "TaskA", 2048, NULL, 5, &taskHandle);
    safety_register_task(taskHandle);

    xTaskCreate(vTaskB, "TaskB", 2048, NULL, 5, &taskHandle);
    safety_register_task(taskHandle);

    xTaskCreate(vSafetyMonitorTask, "Safety", 2048, NULL, 10, NULL);
}
