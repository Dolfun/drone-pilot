#pragma once
#include "Application.h"
#include "Task.hpp"

class ApplicationTask : public Task<4096> {
public:
  ApplicationTask() : Task("application_task", 17, task_function) {}
  
private:
  static void task_function() {
    WifiManager::init();
    
    Application app;
    app.run();
  }
};