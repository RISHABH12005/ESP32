#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"

#define LED_PIN 27
#define LED_COUNT 1

void app_main(void) {
    led_strip_handle_t strip;

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,
        .max_leds = LED_COUNT,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
    };

    led_strip_new_rmt_device(&strip_config, &rmt_config, &strip);

    // Turn LED OFF
    led_strip_set_pixel(strip, 0, 0, 0, 0);
    led_strip_refresh(strip);

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
