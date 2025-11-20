#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"

#define LED_GPIO 27
static const char *TAG = "LED";

void app_main(void)
{
    led_strip_handle_t strip;

    led_strip_config_t config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = false,
        },
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz RMT resolution
    };

    ESP_LOGI(TAG, "Init LED strip");
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&config, &rmt_config, &strip));

    ESP_LOGI(TAG, "Start LED blink");

    while (1) {
        led_strip_set_pixel(strip, 0, 255, 0, 0);  // RED
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_set_pixel(strip, 0, 0, 255, 0);  // GREEN
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_set_pixel(strip, 0, 0, 0, 255);  // BLUE
        led_strip_refresh(strip);
        vTaskDelay(pdMS_TO_TICKS(500));

        led_strip_clear(strip);  // OFF
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
