#pragma once
#include "WifiManager.h"

class Application : public WifiManager::CallbackInterface {
public:
  Application();

  void on_wifi_connect() override;
  void on_wifi_disconnect() override;

  void run();

private:

};