#pragma once
#include "NetworkManager.h"

class Application {
public:
  Application();

  void run();
  
private:
  CommandQueue command_queue;
  NetworkManager network_manager;
};