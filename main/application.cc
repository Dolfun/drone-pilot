#include "application.h"

#include "esp_netif.h"
#include "esp_log.h"

Application::Application() {
  // Start Wi-Fi
  (void)WifiManager::instance();
}