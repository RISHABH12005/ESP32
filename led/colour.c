#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <math.h>

#define LED_GPIO 27
static const char *TAG = "BREATH_RAINBOW";

// Convert HSV → RGB (0–255)
static void hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v,
                       uint8_t *r, uint8_t *g, uint8_t *b)
{
    float r_f = 0, g_f = 0, b_f = 0;    // FIXED: initialized
    float hf = h / 60.0f;
    int   i  = floor(hf);
    float f  = hf - i;

    float p = v * (1 - s / 255.0f);
    float q = v * (1 - f * s / 255.0f);
    float t = v * (1 - (1 - f) * s / 255.0f);

    switch (i % 6)
    {
        case 0: r_f = v; g_f = t; b_f = p; break;
        case 1: r_f = q; g_f = v; b_f = p; break;
        case 2: r_f = p; g_f = v; b_f = t; break;
        case 3: r_f = p; g_f = q; b_f = v; break;
        case 4: r_f = t; g_f = p; b_f = v; break;
        case 5: r_f = v; g_f = p; b_f = q; break;
    }

    *r = (uint8_t)r_f;
    *g = (uint8_t)g_f;
    *b = (uint8_t)b_f;
}

void app_main(void)
{
    led_strip_handle_t strip;

    led_strip_config_t config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {.invert_out = false},
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,   // 10 MHz RMT clock
    };

    ESP_LOGI(TAG, "Initialize LED strip");
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&config, &rmt_config, &strip));

    uint16_t hue = 0;   // 0–360°
    
    while (1)
    {
        // Breathing brightness curve (smooth)
        float t  = fmod((float)esp_timer_get_time() / 1000000.0f, 2.0f);
        float s  = sinf(t * M_PI);        // smooth fade
        float br = (s * s);               // smoother
        uint8_t brightness = (uint8_t)(br * 255);

        // HSV → RGB
        uint8_t r, g, b;
        hsv_to_rgb(hue, 255, brightness, &r, &g, &b);

        // Update LED
        led_strip_set_pixel(strip, 0, r, g, b);
        led_strip_refresh(strip);

        // Gradually change hue (rainbow effect)
        hue = (hue + 2) % 360;

        vTaskDelay(pdMS_TO_TICKS(10));   // very smooth
    }
}
