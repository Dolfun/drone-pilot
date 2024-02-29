#pragma once
#include <atomic>
#include "asio.hpp"

using asio::ip::tcp;

class SocketManager {
public:
  SocketManager(asio::io_context&);

  void connect();
  void disconnect();
  void ping();

private:
  asio::io_context& io_context;
  tcp::resolver::results_type endpoints;
  std::unique_ptr<tcp::socket> socket;
  std::atomic<bool> is_connected = false;
};