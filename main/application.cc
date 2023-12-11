#include "Application.h"
#include "WifiManager.h"

Application::Application() : wifi_manager { WifiManager::instance() } {
  wifi_manager.register_on_connect_callback( [this] {
    on_wifi_connect();
  });

  wifi_manager.register_on_disconnect_callback( [this] {
    on_wifi_disconnect();
  });

  wifi_manager.start();

  if (connection_manager == nullptr) {
    connection_manager = std::make_unique<ConnectionManager>();
  }
}

void Application::on_wifi_connect() {
  connection_manager->set_auto_reconnect(true);
  connection_manager->connect();
}

void Application::on_wifi_disconnect() {
  connection_manager->set_auto_reconnect(false);
  connection_manager->disconnect();
}