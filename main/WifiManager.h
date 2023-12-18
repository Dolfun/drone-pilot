#pragma once
#include "esp_event.h"
namespace WifiManager {
  void init();
  void start();

  void register_on_connect_callback(void*, esp_event_handler_t);
  void register_on_disconnect_callback(void*, esp_event_handler_t);
};