#include "WifiManager.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include <cstring>

#define WIFI_LOG_TAG "wifi_manager"

char wifi_ssid[32] = CONFIG_ESP_WIFI_SSID;
char wifi_pass[64] = CONFIG_ESP_WIFI_PASSWORD;

void WifiManager::init() {
  // Initialize non-volatile storage
  auto error = nvs_flash_init();
  if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    error = nvs_flash_init();
  }
  ESP_ERROR_CHECK(error);
  
  // Initialize netif
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  // Configure Wi-Fi
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

  wifi_sta_config_t wifi_sta_config {};
  wifi_sta_config.threshold.authmode = WIFI_AUTH_WPA_PSK;
  std::memcpy(wifi_sta_config.ssid, wifi_ssid, sizeof(wifi_ssid));
  std::memcpy(wifi_sta_config.password, wifi_pass, sizeof(wifi_pass));
  
  wifi_config_t wifi_config = { .sta { wifi_sta_config } };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

  // Register event handles
  auto start_callback = [] (void*, esp_event_base_t, int32_t, void*) {
    esp_wifi_connect();
  };

  auto connect_callback = [] (void*, esp_event_base_t, int32_t, void* event_data) {
    auto event = static_cast<ip_event_got_ip_t*>(event_data);
    char ip_address[16];
    sprintf(ip_address, IPSTR, IP2STR(&event->ip_info.ip));
    ESP_LOGI(WIFI_LOG_TAG, "wifi connected to: %s", ip_address);
  };

  auto disconnect_callback = [] (void*, esp_event_base_t, int32_t, void*) {
      ESP_LOGI(WIFI_LOG_TAG, "retrying to connect...");
    esp_wifi_connect();
    };

  esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_START, start_callback, nullptr, nullptr);
  esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, connect_callback, nullptr, nullptr);
  esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, disconnect_callback, nullptr, nullptr);
}

void WifiManager::start() {
  ESP_ERROR_CHECK(esp_wifi_start());
}