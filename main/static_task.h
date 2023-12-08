#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>
#include <array>

template <size_t N>
class StaticTask {
public:
  StaticTask(TaskFunction_t func, void* arg, std::string name, UBaseType_t priority) {
    task_handle = 
      xTaskCreateStatic(
        func,
        name.c_str(),
        static_cast<uint32_t>(N),
        arg,
        priority,
        stack.data(),
        &task_buffer
      );
  }

  TaskHandle_t handle() const {
    return task_handle;
  }
private:
  StaticTask_t task_buffer;
  std::array<StackType_t, N> stack;

  TaskHandle_t task_handle = nullptr;
};