#pragma once
#include "Task.hpp"
#include "NetworkManager.h"
#include <memory>

class NetworkingTask : public TaskInterface<4096> {
public:
  NetworkingTask() : TaskInterface("networking_task", 17) {}

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