#pragma once
#include "ConnectionManager.h"

class Application {
public:
  Application();

  void run();
  
private:
  ConnectionManager connection_manager;
};