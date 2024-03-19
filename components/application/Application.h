#pragma once
#include "Server.h"

class Application {
public:
  void run();
  
private:
  Websocket::Server server;
};