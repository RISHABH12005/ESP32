#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <math.h>

#define LED_PIN 27     // Your board’s RGB LED pin
#define LED_COUNT 1    // Only 1 LED
static const char *TAG = "BREATH";

// Smooth brightness 0 → 1 → 0
static float breathing_wave(float t) {
    return 0.5f * (1.0f - cosf(2.0f * M_PI * t));   // cosine wave
}

// Fade color with brightness
static void scale_color(uint8_t base_r, uint8_t base_g, uint8_t base_b,
                        float brightness,
                        uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (uint8_t)(base_r * brightness);
    *g = (uint8_t)(base_g * brightness);
    *b = (uint8_t)(base_b * brightness);
}

void app_main(void) {
    ESP_LOGI(TAG, "Init LED Strip");

    led_strip_handle_t strip;
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,
        .max_leds = LED_COUNT,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,   // 10MHz
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &strip));
    ESP_LOGI(TAG, "LED Ready");

    // Color cycle: WHITE → RED → GREEN
    uint8_t color_cycle[][3] = {
        {255, 255, 255},  // white
        {255,   0,   0},  // red
        {  0, 255,   0}   // green
    };

    int color_index = 0;

    while (1) {
        // 3-second breathing cycle
        float t = (esp_timer_get_time() % 3000000ULL) / 3000000.0f;
        float brightness = breathing_wave(t);

        uint8_t r, g, b;
        scale_color(
            color_cycle[color_index][0],
            color_cycle[color_index][1],
            color_cycle[color_index][2],
            brightness,
            &r, &g, &b
        );

        led_strip_set_pixel(strip, 0, r, g, b);
        led_strip_refresh(strip);

        // When brightness hits bottom → next color
        if (brightness < 0.01f) {
            color_index = (color_index + 1) % 3;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // smooth animation
    }
}
