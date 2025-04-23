volatile uint8_t taskA_ok = 0;
volatile uint8_t taskB_ok = 0;

void vSafetyMonitorTask(void *pvParameters) {
    while (1) {
        if (!taskA_ok || !taskB_ok) {
            printf("Critical Task Failure\n");
            esp_restart();  // Reboot in safe mode 
        }

        // Reset flags
        taskA_ok = 0;
        taskB_ok = 0;

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
