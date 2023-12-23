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
class StaticTask {
public:
  StaticTask(const char* name, UBaseType_t priority, void* arg, TaskFunction_t handle) {
    task_handle = 
      xTaskCreateStatic(
        handle,
        name,
        static_cast<uint32_t>(N),
        arg,
        priority,
        stack.data(),
        &task_buffer
      );
  }

  StaticTask(const char* name, TaskAffinity affinity, UBaseType_t priority, void* arg, TaskFunction_t handle) {
    task_handle = 
      xTaskCreateStaticPinnedToCore(
        handle,
        name,
        static_cast<uint32_t>(N),
        arg,
        priority,
        stack.data(),
        &task_buffer,
        static_cast<UBaseType_t>(affinity)
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