#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <optional>
#include <cstddef>
#include <array>

template <typename T, std::size_t N>
class Queue {
public:
  Queue() {
    handle = xQueueCreateStatic(
      static_cast<UBaseType_t>(N),
      static_cast<UBaseType_t>(sizeof(T)),
      storage_buffer.data(),
      &queue_buffer
    );
  }

  ~Queue() {
    vQueueDelete(handle);
  }

  Queue(const Queue&) = delete;
  Queue& operator= (const Queue&) = delete;
  Queue(Queue&&) = default;
  Queue& operator= (Queue&&) = default;

  auto peek() const -> std::optional<T> {
    return peek_impl(static_cast<TickType_t>(0));
  }

  auto peek_until() const -> std::optional<T> {
    return peek_impl(portMAX_DELAY);
  }

  auto pop() -> std::optional<T> {
    return pop_impl(static_cast<TickType_t>(0));
  }

  auto pop_until() -> std::optional<T> {
    return pop_impl(portMAX_DELAY);
  }

  bool push(const T& item) {
    auto error = xQueueSendToBack(handle, static_cast<const void*>(&item), 0);
    return (error == pdTRUE);
  }

  std::size_t size() const {
    return static_cast<std::size_t>(uxQueueMessagesWaiting(handle));
  }

  std::size_t free_size() const {
    return static_cast<std::size_t>(uxQueueSpacesAvailable(handle));
  }

  std::size_t capacity() const {
    return N;
  }

  bool empty() const {
    return (free_size() == 0);
  }

  void reset() {
    xQueueReset(handle);
  }

private:
  QueueHandle_t handle;
  std::array<uint8_t, N * sizeof(T)> storage_buffer;
  StaticQueue_t queue_buffer;

  auto peek_impl(TickType_t ticks_to_wait) const -> std::optional<T> {
    T item;
    if (xQueuePeek(handle, static_cast<void*>(&item), ticks_to_wait)) {
      return item;
    } else {
      return std::nullopt;
    }
  }

  auto pop_impl(TickType_t ticks_to_wait) -> std::optional<T> {
    T item;
    if (xQueueReceive(handle, static_cast<void*>(&item), ticks_to_wait)) {
      return item;
    } else {
      return std::nullopt;
    }
  }
};