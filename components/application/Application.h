#pragma once
#include "Wifi.h"

class Application : public Wifi::CallbackInterface {
public:
  Application();

  void on_wifi_connect() override;
  void on_wifi_disconnect() override;

  void run();

private:

};