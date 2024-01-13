#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <array>

enum class TaskAffinity : UBaseType_t {
  CPU0 = 0,
  CPU1 = 1,
  CPUx = tskNO_AFFINITY,
};

template <size_t N>
class Task {
public:
  Task(const char* name, UBaseType_t priority, TaskFunction_t task_function, void* arg) {
    handle = xTaskCreateStatic(
      task_function,
      name,
      static_cast<uint32_t>(N),
      arg,
      priority,
      stack_buffer.data(),
      &task_buffer
    );
  }

  Task(const char* name, UBaseType_t priority, TaskAffinity affinity, TaskFunction_t task_function, void* arg) {
    handle = xTaskCreateStaticPinnedToCore(
      task_function,
      name,
      static_cast<uint32_t>(N),
      arg,
      priority,
      stack_buffer.data(),
      &task_buffer,
      static_cast<UBaseType_t>(affinity)
    );
  }

  virtual ~Task() {
    vTaskDelete(handle);
  }

  Task(const Task&) = delete;
  Task& operator= (const Task&) = delete;
  Task(Task&&) = default;
  Task& operator= (Task&&) = default;

protected:
  TaskHandle_t handle;

private:
  StaticTask_t task_buffer;
  std::array<StackType_t, N> stack_buffer;
};

template <size_t N>
class TaskInterface : public Task<N> {
public:
  TaskInterface(const char* name, UBaseType_t priority)
    : Task<N> { name, priority, task_function_wrapper, static_cast<void*>(this) } {}

private:
  virtual void task_function() = 0;

  static void task_function_wrapper(void* arg) {
    auto& self = *static_cast<TaskInterface<N>*>(arg);
    self.task_function();
  }
};