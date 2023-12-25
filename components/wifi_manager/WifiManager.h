#pragma once
#include <functional>
#include "esp_wifi.h"

namespace WifiManager {

void init();
void start();

class EventCallback {
public:
  template <typename Callback>
  EventCallback(esp_event_base_t event_base, int32_t event_id, Callback callback) 
   : _event_base { event_base }, _event_id { event_id }, _callback { callback } {
    esp_event_handler_instance_register(
      _event_base, _event_id, callback_wrapper, static_cast<void*>(this), &instance);
  }

  ~EventCallback() {
    esp_event_handler_instance_unregister(_event_base, _event_id, instance);
  }

  EventCallback(const EventCallback&) = delete;
  EventCallback& operator= (const EventCallback&) = delete;
  EventCallback(EventCallback&&) = default;
  EventCallback& operator= (EventCallback&&) = default;

private:
  esp_event_base_t _event_base;
  int32_t _event_id;
  std::function<void()> _callback;
  esp_event_handler_instance_t instance { nullptr };

  static void callback_wrapper(void* arg, esp_event_base_t, int32_t, void*) {
    auto& self = *static_cast<EventCallback*>(arg);
    self._callback();
  }
};

class ConnectCallback : public EventCallback {
public:
  template <typename Callback>
  ConnectCallback(Callback callback) : EventCallback { IP_EVENT, IP_EVENT_STA_GOT_IP, callback } {}
};

class DisconnectCallback : public EventCallback {
public:
  template <typename Callback>
  DisconnectCallback(Callback callback) : EventCallback { WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, callback } {}
};

};