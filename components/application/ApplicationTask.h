#pragma once
#include "Application.h"
#include "Task.hpp"

class ApplicationTask : public TaskInterface<4096> {
public:
  ApplicationTask() : TaskInterface("application_task", 17) {}
  
private:
  void task_function() override {
    WifiManager::init();
    
    Application app;
    app.run();
  }
};