#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BLINK_GPIO 2  // Usually onboard LED on ESP32-C5 DevKit, change if needed

void app_main(void)
{
    // Configure the GPIO
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    ESP_LOGI("BLINK", "Starting Blink LED example");

    while (1) {
        gpio_set_level(BLINK_GPIO, 1); // LED ON
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 0); // LED OFF
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
