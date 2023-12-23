#pragma once
#include "WifiManager.h"
#include "StaticTask.h"
#include <memory>

class Application {
public:
  Application();

  void on_wifi_connect();
  void on_wifi_disconnect();
  // void on_message_receive(MessageData&&);
private:
  std::unique_ptr<WifiManager::ConnectCallback> wifi_connect_callback;
  std::unique_ptr<WifiManager::DisconnectCallback> wifi_disconnect_callback;
  // std::unique_ptr<ConnectionManager> connection_manager{};
};

class ApplicationTask : public StaticTask<4096> {
public:
  ApplicationTask()
    : StaticTask("application_task", 17, nullptr, handle) {}
private:
  static void handle(void*) {
    Application app;

    for (;;) {
      vTaskDelay(pdTICKS_TO_MS(500));
    }
  }
};