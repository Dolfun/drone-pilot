#include "Application.h"
#include "Wifi.h"

void Application::run() {
  Wifi::start();
  
  for (;;) {
    vTaskDelay(pdTICKS_TO_MS(500));
  }
}