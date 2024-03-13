#pragma once
#include "asio.hpp"

using asio::ip::tcp;

class Session {
public:
  Session(tcp::socket _socket) : socket(std::move(_socket)) {}
  
private:
  tcp::socket socket;
};