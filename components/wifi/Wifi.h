#pragma once
#include "esp_wifi.h"

namespace Wifi {

void init();
void start();

class Interface {
public:
  Interface() {
    esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP,
      on_connect_callback_wrapper,
      static_cast<void*>(this),
      &on_connect_handle
    );

    esp_event_handler_instance_register(
      WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED,
      on_disconnect_callback_wrapper,
      static_cast<void*>(this),
      &on_disconnect_handle
    );
  }

  virtual ~Interface() {
    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, on_connect_handle);
    esp_event_handler_instance_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, on_disconnect_handle);
  }

  Interface(const Interface&) = delete;
  Interface& operator= (const Interface&) = delete;
  Interface(Interface&&) = default;
  Interface& operator= (Interface&&) = default;

  virtual void on_wifi_connect() = 0;
  virtual void on_wifi_disconnect() = 0;
private:
  esp_event_handler_instance_t on_connect_handle, on_disconnect_handle;

  void static on_connect_callback_wrapper(void* arg, esp_event_base_t, int32_t, void*) {
    auto& self = *static_cast<Interface*>(arg);
    self.on_wifi_connect();
  }

  void static on_disconnect_callback_wrapper(void* arg, esp_event_base_t, int32_t, void*) {
    auto& self = *static_cast<Interface*>(arg);
    self.on_wifi_disconnect();
  }
};

};