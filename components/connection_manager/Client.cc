#include "Client.h"
#include "esp_log.h"

const char* ip_address { CONFIG_ESP_MASTER_IP_ADDRESS };
const char* port_no    { CONFIG_ESP_MASTER_PORT_NO    };

#define LOG_TAG "Client"

Client::Client(asio::io_context& _io_context)
    : io_context { _io_context }, socket { io_context } {}

void Client::connect() {
  if (is_connected) return;

  static tcp::resolver resolver { io_context };
  static auto endpoints = resolver.resolve(ip_address, port_no);

  asio::async_connect(socket, endpoints, [this] (asio::error_code ec, tcp::endpoint) mutable {
    if (ec) {
      ESP_LOGE(LOG_TAG, "%s", ec.message().c_str());
      if (to_retry) connect();
    } else {
      is_connected = true;
      session = std::make_unique<Session>(std::move(socket));
      ESP_LOGI(LOG_TAG, "connected!");
    }
  });
}

void Client::disconnect() {
  is_connected = false;
  session.reset();
}

void Client::on_wifi_connect() {
  to_retry = true;
  connect();
}

void Client::on_wifi_disconnect() {
  to_retry = false;
  disconnect();
}