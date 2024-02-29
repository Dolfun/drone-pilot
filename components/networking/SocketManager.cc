#include "SocketManager.h"
#include "esp_log.h"

const char* ip_address { CONFIG_ESP_MASTER_IP_ADDRESS };
const char* port_no    { CONFIG_ESP_MASTER_PORT_NO    };

SocketManager::SocketManager(asio::io_context& _io_context)
  : io_context(_io_context) {
  tcp::resolver resolver { io_context };
  endpoints = resolver.resolve(ip_address, port_no);
}

void SocketManager::connect() {
  if (is_connected) return;
  socket = std::make_unique<tcp::socket>(io_context);

  asio::async_connect(*socket, endpoints, [this] (asio::error_code ec, tcp::endpoint) {
    if (ec) {
      ESP_LOGE("asio", "async_connect: %s", ec.message().c_str());
      connect();
    } else {
      is_connected = true;
    }
  });
}

void SocketManager::disconnect() {
  is_connected = false;
}

void ping() {

}