#pragma once
#include "asio.hpp"
#include "Command.hpp"
#include "Queue.hpp"

class CommandQueue {
public:
  auto get() {
    return queue.pop_until();
  }
  
private:
  friend class Receiver;
  Queue<Command, 32> queue;
};

class CommandReceiver {
public:
  CommandReceiver(asio::io_context& _io_context, CommandQueue& _command_queue)
    : io_context(_io_context), command_queue(_command_queue) {}

private:
  asio::io_context& io_context;
  CommandQueue& command_queue;
};