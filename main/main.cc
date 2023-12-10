#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "static_task.h"
#include "application.h"

#define LED_PIN GPIO_NUM_2
void blink_led(void*) {
  esp_rom_gpio_pad_select_gpio(LED_PIN);
  ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));
  bool led_state = false;
  for (;;) {
    ESP_ERROR_CHECK(gpio_set_level(LED_PIN, led_state));
    led_state = !led_state;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_task_handle(void*) {
  Application app;

  for (;;) {
    vTaskDelay(pdTICKS_TO_MS(100));
  }
}

extern "C" void app_main() {
  static StaticTask<512> led_task{ "blink_led", 10, nullptr, blink_led };
  static StaticTask<4096> application_task{ "application_task", 15, nullptr, app_task_handle };
}