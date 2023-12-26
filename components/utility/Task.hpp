#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <functional>
#include <array>

enum class TaskAffinity : UBaseType_t {
  CPU0 = 0,
  CPU1 = 1,
  CPUx = tskNO_AFFINITY,
};

template <size_t N>
class Task {
public:
  template <typename TaskFunction>
  Task(const char* name, UBaseType_t priority, TaskFunction task_function)
   : _task_function(task_function) {
    task_handle = xTaskCreateStatic(
      task_function_wrapper,
      name,
      static_cast<uint32_t>(N),
      static_cast<void*>(this),
      priority,
      stack.data(),
      &task_buffer
    );
  }

  template <typename TaskFunction>
  Task(const char* name, UBaseType_t priority, TaskAffinity affinity, TaskFunction task_function)
   : _task_function(task_function) {
    task_handle = xTaskCreateStaticPinnedToCore(
      task_function_wrapper,
      name,
      static_cast<uint32_t>(N),
      static_cast<void*>(this),
      priority,
      stack.data(),
      &task_buffer,
      static_cast<UBaseType_t>(affinity)
    );
  }

  ~Task() {
    vTaskDelete(task_handle);
  }

  Task(const Task&) = delete;
  Task& operator= (const Task&) = delete;
  Task(Task&&) = default;
  Task& operator= (Task&&) = default;

protected:
  TaskHandle_t task_handle = nullptr;

private:
  std::function<void()> _task_function;
  StaticTask_t task_buffer {};
  std::array<StackType_t, N> stack {};

  static void task_function_wrapper(void* arg) {
    auto& self = *static_cast<Task<N>*>(arg);
    self._task_function();
  }
};