#pragma once
#include <atomic>
#include "asio.hpp"
#include "Wifi.h"
#include "Session.h"

using asio::ip::tcp;

class ConnectionManager : public Wifi::Interface {
public:
  ConnectionManager(asio::io_context&);

  void on_wifi_connect() override;
  void on_wifi_disconnect() override;

private:
  asio::io_context& io_context;
  tcp::socket socket;
  std::unique_ptr<Session> session;
  std::atomic<bool> is_connected, to_retry;

  void connect();
  void disconnect();
};