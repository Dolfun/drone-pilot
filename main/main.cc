#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "Application.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
void blink_led() {
  esp_rom_gpio_pad_select_gpio(LED_PIN);
  ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
  bool led_state = false;
  for (;;) {
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, led_state));
    led_state = !led_state;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static Task<512> led_task { "blink_led", 10, blink_led };
static ApplicationTask application_task {};

extern "C" void app_main() {
  for (;;) {
    ESP_LOGI("main", "free_heap_size: %luKiB", esp_get_free_heap_size() / 1024);
    ESP_LOGI("main", "largerst_free_block_size: %uKiB", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) / 1024);
    ESP_LOGI("main", "minimum_free_size: %luKiB", esp_get_minimum_free_heap_size() / 1024);

    vTaskDelay(pdMS_TO_TICKS(10'000));
  }
}