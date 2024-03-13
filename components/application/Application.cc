#include "Application.h"
#include "ConnectionTask.h"
#include "Wifi.h"

static ConnectionTask connection_task;

Application::Application() 
  : client(connection_task.get_io_context()) {}

void Application::run() {
  Wifi::start();
  
  for (;;) {
    vTaskDelay(pdTICKS_TO_MS(500));
  }
}