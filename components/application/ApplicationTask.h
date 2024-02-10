#pragma once
#include "Application.h"
#include "Task.hpp"
#include "Wifi.h"

class ApplicationTask : public TaskInterface<4096> {
public:
  ApplicationTask() : TaskInterface("application_task", 17) {}
  
private:
  void task_function() override {
    Wifi::init();

    Application app;
    app.run();
  }
};