#include "Application.h"

Application::Application() {
  Wifi::start();
}

void Application::on_wifi_connect() {

}

void Application::on_wifi_disconnect() {
  
}

void Application::run() {
  for (;;) {
    vTaskDelay(pdTICKS_TO_MS(500));
  }
}