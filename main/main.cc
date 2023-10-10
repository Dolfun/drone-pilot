#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

void blink_led(void* args) {
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
    bool led_state = false;
    for (;;) {
        ESP_ERROR_CHECK(gpio_set_level(LED_PIN, led_state));
        led_state = !led_state;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void app_main() {
    xTaskCreate(blink_led, "blink_led", 4096, nullptr, 10, nullptr);
}