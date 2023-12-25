#pragma once
#include "WifiManager.h"
#include "Task.h"
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

class ApplicationTask : public Task<4096> {
public:
  ApplicationTask() : Task("application_task", 17, task_function) {}
  
private:
  static void task_function() {
    Application app;

    for (;;) {
      vTaskDelay(pdTICKS_TO_MS(500));
    }
  }
};