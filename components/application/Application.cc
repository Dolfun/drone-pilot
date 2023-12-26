#include "Application.h"
#include "WifiManager.h"

Application::Application() {
  WifiManager::init();
  wifi_connect_callback = std::make_unique<WifiManager::ConnectCallback>(
      std::bind(&Application::on_wifi_connect, this));
  wifi_disconnect_callback = std::make_unique<WifiManager::DisconnectCallback>(
      std::bind(&Application::on_wifi_disconnect, this));
  WifiManager::start();
}

void Application::on_wifi_connect() {

}

void Application::on_wifi_disconnect() {
  
}