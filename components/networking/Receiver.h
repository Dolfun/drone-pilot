#pragma once
#include "Command.hpp"
#include "Queue.hpp"
#include "asio.hpp"

using asio::ip::tcp;
using CommandQueue = Queue<Command, 32>;

template <std::size_t N>
class Receiver {
public:
  Receiver(asio::io_context& _io_context, tcp::socket& _socket, CommandQueue& _queue)
    : io_context(_io_context) {}

private:
  asio::io_context& io_context;
};