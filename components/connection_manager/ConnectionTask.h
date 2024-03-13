#pragma once
#include "Task.hpp"
#include "Client.h"

class ConnectionTask : public TaskInterface<4096> {
public:
  ConnectionTask() : TaskInterface("connection_task", 17) {}

  asio::io_context& get_io_context() {
    return io_context;
  }
  
private:
  void task_function() override {
    auto work = asio::make_work_guard(io_context);
    io_context.run();
  }

  asio::io_context io_context;
};