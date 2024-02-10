#pragma once
#include "asio.hpp"
#include "Receiver.h"
#include "Sender.h"

class NetworkManager {
public:
  NetworkManager(asio::io_context& _io_context, CommandQueue& _command_queue)
    : io_context(_io_context), command_queue(_command_queue) {}

private:
  asio::io_context& io_context;
  CommandQueue& command_queue;
};