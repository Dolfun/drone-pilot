#pragma once

#include "ConnectionManager.h"
#include "WifiManager.h"
#include "esp_event.h"
#include <memory>

class Application {
public:
  Application();

  void on_wifi_connect();
  void on_wifi_disconnect();
private:
  WifiManager& wifi_manager;
  std::unique_ptr<ConnectionManager> connection_manager {};
};