#pragma once

#include "wifi_manager.h"
#include "esp_event.h"

class Application : public WifiObserver {
public:
  Application();

  void on_connect() override {
  }

  void on_disconnect() override {
  }
private:

};