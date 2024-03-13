#pragma once
#include "Client.h"

class Application {
public:
  Application();

  void run();
  
private:
  Client client;
};