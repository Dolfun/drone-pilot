#include "wifi_manager.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include <algorithm>
#include <string>

const std::string wifi_ssid { CONFIG_ESP_WIFI_SSID     };
const std::string wifi_pass { CONFIG_ESP_WIFI_PASSWORD };

bool WifiManager::is_connected { false };
bool WifiManager::once_flag    { true  };
WifiManagerCallback WifiManager::on_connect_callback    { do_nothing };
WifiManagerCallback WifiManager::on_disconnect_callback { do_nothing };

WifiManager::WifiManager() {
  auto error = nvs_flash_init();
  if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    error = nvs_flash_init();
  }
  ESP_ERROR_CHECK(error);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

  wifi_scan_threshold_t wifi_scan_threshold {};
  wifi_scan_threshold.authmode = WIFI_AUTH_WPA_PSK;

  wifi_sta_config_t wifi_sta_config {};
  std::copy(wifi_ssid.begin(), wifi_ssid.end(), wifi_sta_config.ssid);
  std::copy(wifi_pass.begin(), wifi_pass.end(), wifi_sta_config.password);
  wifi_sta_config.threshold = wifi_scan_threshold;

  wifi_config_t wifi_config = {
    .sta{wifi_sta_config}
  };

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_START, 
                  wifi_start_callback, nullptr, nullptr));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, 
                  ip_get_callback, nullptr, nullptr));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, 
                  wifi_disconnect_callback, nullptr, nullptr));

  ESP_ERROR_CHECK(esp_wifi_start());
}

void WifiManager::wifi_start_callback(void*, esp_event_base_t, int32_t, void*) {
  esp_wifi_connect();
}

void WifiManager::ip_get_callback(void*, esp_event_base_t, int32_t, void* event_data) {
  auto event = static_cast<ip_event_got_ip_t*>(event_data);
  char ip_address[16];
  sprintf(ip_address, IPSTR, IP2STR(&event->ip_info.ip));
  ESP_LOGI(WIFI_LOG_TAG, "got ip: %s", ip_address);

  is_connected = true;
  once_flag = true;
  on_connect_callback();
}

void WifiManager::wifi_disconnect_callback(void*, esp_event_base_t, int32_t, void*) {
  is_connected = false;

  if (once_flag) {
    once_flag = false;
    on_disconnect_callback();
  }
  
  ESP_LOGI(WIFI_LOG_TAG, "retrying to connect");
  esp_wifi_connect();
}