#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include <functional>

#define WIFI_LOG_TAG "wifi_manager"

using WifiManagerCallback = std::function<void()>;
class WifiManager {
public:
  WifiManager(WifiManager&) = delete;
  WifiManager& operator=(const WifiManager&) = delete;
  WifiManager(WifiManager&&) = delete;
  WifiManager& operator=(WifiManager&&) = delete;

  static WifiManager& instance() {
    static WifiManager wifi_manager;
    return wifi_manager;
  }

  void register_on_connect_callback(WifiManagerCallback callback) {
    on_connect_callback = callback;
  }

  void register_on_disconnect_callback(WifiManagerCallback callback) {
    on_disconnect_callback = callback;
  }

  void start();

  bool connected() const { 
    return is_connected;
  }
private:
  bool is_connected { false };
  bool once_flag    { false };

  WifiManager();

  WifiManagerCallback on_connect_callback{};
  WifiManagerCallback on_disconnect_callback{};

  static void wifi_start_callback(void*, esp_event_base_t, int32_t, void*);
  static void ip_get_callback(void*, esp_event_base_t, int32_t, void*);
  static void wifi_disconnect_callback(void*, esp_event_base_t, int32_t, void*);
};