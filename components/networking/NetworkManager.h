#pragma once
#include "asio.hpp"
#include "SocketManager.h"
#include "CommandReceiver.h"
#include "Wifi.h"

class NetworkManager : public Wifi::Interface {
public:
  NetworkManager(asio::io_context& io_context, CommandQueue& command_queue)
    : socket_manager(io_context), command_receiver(io_context, command_queue) {}

  void on_wifi_connect() override;
  void on_wifi_disconnect() override;

private:
  SocketManager socket_manager;
  CommandReceiver command_receiver;
};