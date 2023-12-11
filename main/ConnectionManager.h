#pragma once

#include "esp_log.h"
#include "asio.hpp"
#include <cstddef>
#include <vector>
#include <array>
#include <semaphore>

using asio::ip::tcp;
using MessageContainer = std::vector<std::byte>;

constexpr std::size_t max_message_length { 4096 };

class ConnectionManager {
public:
  ConnectionManager();
  
  void connect();
  void disconnect();
  void set_auto_reconnect(bool);
  void send_message(MessageContainer&&);
private:
  struct Message;
  asio::io_context io_context;
  std::unique_ptr<tcp::socket> socket{};
  tcp::resolver::results_type endpoints;
  bool connected { false };
  bool to_auto_reconnect { false };

  std::array<std::vector<Message>, 2> messages_array{};
  std::atomic<uint32_t> index { 0 };
  std::binary_semaphore is_sending_signal { 1 };
  
  void on_connect();
  void process_queue();
  bool is_connection_error(const asio::error_code&) const;
};

struct ConnectionManager::Message {
  Message() = default;
  Message(MessageContainer&&);

  std::array<std::byte, 2> length_data{};
  MessageContainer data;
};