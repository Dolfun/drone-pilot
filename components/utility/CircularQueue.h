#pragma once
#include <cstddef>
#include <utility>
#include <array>

template <typename T, size_t N>
class circular_queue {
public:
  constexpr const T& front() const {
    if (is_empty()) {
        throw std::out_of_range("queue is empty");
    }
    return data[_front];
  }

  constexpr T& front() {
    return const_cast<T&>(std::as_const(*this).front());
  }

  constexpr bool enqueue(const T& val) {
    if (is_full()) {
      return false;
    }
    data[_rear] = val;
    _rear = (_rear + 1) % N;
    ++_size;
    return true;
  }

  constexpr bool dequeue() {
    if (is_empty()) {
      return false;
    }
    _front = (_front + 1) % N;
    --_size;
    return true;
  }

  constexpr size_t size() const noexcept {
    return _size;
  }

  constexpr bool is_empty() const noexcept {
    return (_size == 0);
  }

  constexpr bool is_full() const noexcept {
    return (_size == N);
  }
private:
  std::array<T, N> data{};
  ptrdiff_t _front{}, _rear{};
  size_t _size{};
};
