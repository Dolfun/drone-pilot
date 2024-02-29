#include "Application.h"
#include "NetworkingTask.h"
#include "Wifi.h"

static NetworkingTask networking_task;

Application::Application() 
  : network_manager(networking_task.get_io_context(), command_queue) {}

void Application::run() {
  Wifi::start();
  
  for (;;) {
    vTaskDelay(pdTICKS_TO_MS(500));
  }
}