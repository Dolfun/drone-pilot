#include "ConnectionManager.h"
#include "StaticTask.h"
#include "esp_log.h"

#define ASIO_LOG_TAG "asio"

const char* ip_address { CONFIG_ESP_MASTER_IP_ADDRESS };
const char* port_no    { CONFIG_ESP_MASTER_PORT_NO    };

ConnectionManager::ConnectionManager() {
  static StaticTask<4096> io_context_task {
    "io_context_task", 16,
    static_cast<void*>(&io_context), 
    [] (void* io_context_ptr) {
      auto& io_context = *static_cast<asio::io_context*>(io_context_ptr);
      auto work = asio::make_work_guard(io_context);
      for(;;) {
        try {
          io_context.run();
        } catch (std::exception& e) {
          ESP_LOGE(ASIO_LOG_TAG, "io_context.run(): %s", e.what());
        }
      }
    }
  };

  tcp::resolver resolver { io_context };
  endpoints = resolver.resolve(ip_address, port_no);
}

void ConnectionManager::connect() {
  if (connected) return;
  try {
    socket = std::make_unique<tcp::socket>(io_context);
  } catch (const std::exception& e) {
    ESP_LOGE(ASIO_LOG_TAG, "start(): %s", e.what());
  }

  asio::async_connect(*socket, endpoints, [this] (const asio::error_code& ec, tcp::endpoint) {
    if (ec) {
      ESP_LOGE(ASIO_LOG_TAG, "async_connect: %s", ec.message().c_str());
      if (is_connection_error(ec)) {
        disconnect();
      }
    } else {
      ESP_LOGI(ASIO_LOG_TAG, "Socket connected!");
      connected = true;
      on_connect();
    }
  });
}

void ConnectionManager::disconnect() {
  connected = false;
  try {
    socket.reset();
  } catch (const std::exception& e) {
    ESP_LOGE(ASIO_LOG_TAG, "socket->close(): %s", e.what());
  }

  if (to_auto_reconnect) connect();
}

void ConnectionManager::set_auto_reconnect(bool b) {
  to_auto_reconnect = b;
}

void ConnectionManager::send_message(MessageContainer&& data) {
  messages_array[index].emplace_back(std::move(data));
  ESP_LOGI(ASIO_LOG_TAG, "messages_array -> (%d, %d)", messages_array[0].size(), messages_array[1].size());
  process_queue();
}

void ConnectionManager::on_connect() {
  process_queue();
}

void ConnectionManager::process_queue() {
  asio::post(io_context, [this] {
    if (!connected) return;

    const auto& messages = messages_array[index];
    if (messages.empty()) return;

    if (!is_sending_signal.try_acquire()) return;
    index ^= 1;

    std::vector<asio::const_buffer> buffers;
    buffers.reserve(messages.size() * 2);
    for (const auto& message : messages) {
      buffers.emplace_back(asio::buffer(message.length_data));
      buffers.emplace_back(asio::buffer(message.data));
    }

    asio::async_write(*socket, buffers, 
      [this] (const asio::error_code& ec, std::size_t nr_bytes) {
        is_sending_signal.release();
        if (ec) {
          ESP_LOGE(ASIO_LOG_TAG, "async_write: %s", ec.message().c_str());

          if (is_connection_error(ec)) {
            disconnect();
          }
        } else {
          ESP_LOGI(ASIO_LOG_TAG, "async_write: %d bytes written", nr_bytes);
          messages_array[index ^ 1].clear();
          process_queue();
        }
      }
    );
  });
}

bool ConnectionManager::is_connection_error(const asio::error_code& ec) const {
  return ec == asio::error::connection_aborted ||
         ec == asio::error::connection_reset   ||
         ec == asio::error::not_connected;
}

ConnectionManager::Message::Message(MessageContainer&& _data) : data(std::move(_data)) {
  if (data.size() > max_message_length) data.resize(max_message_length);
  auto length = static_cast<uint16_t>(std::min(data.size(), max_message_length));
  
  uint16_t mask = (1 << 4) - 1;
  std::byte lower_byte{ static_cast<uint8_t>(length & mask) };
  std::byte upper_byte{ static_cast<uint8_t>((length >> 4) & mask) };
  length_data = { lower_byte, upper_byte };
}