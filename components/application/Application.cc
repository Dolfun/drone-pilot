#include "Application.h"
#include "WifiManager.h"

Application::Application() {
  WifiManager::init();
  wifi_connect_callback = std::make_unique<WifiManager::ConnectCallback>(
      std::bind(&Application::on_wifi_connect, this));
  wifi_disconnect_callback = std::make_unique<WifiManager::DisconnectCallback>(
      std::bind(&Application::on_wifi_disconnect, this));
  WifiManager::start();

  // if (connection_manager == nullptr) {
  //   connection_manager = std::make_unique<ConnectionManager>(
  //     std::bind(&Application::on_message_receive, this, std::placeholders::_1)
  //   );
  // }
}

void Application::on_wifi_connect() {
  // connection_manager->set_auto_reconnect(true);
  // connection_manager->connect();
}

void Application::on_wifi_disconnect() {
  // connection_manager->set_auto_reconnect(false);
  // connection_manager->disconnect();
}

// void Application::on_message_receive(MessageData&& message) {
//   ESP_LOGI("application", "message received: %d bytes", message.size());
// }