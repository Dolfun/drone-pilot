#pragma once
#include "NetworkManager.h"
#include <memory>

class Application {
public:
  Application();

  void run();
  
private:
  CommandQueue command_queue;
  std::unique_ptr<NetworkManager> network_manager;
};