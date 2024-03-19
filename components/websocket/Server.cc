#include "Server.h"
#include <esp_log.h>
#include <array>

#define TAG "websocket"
constexpr std::size_t payload_size = 32;

namespace Websocket {

  void Server::on_wifi_connect() {
    start();
  }

  void Server::on_wifi_disconnect() {
    stop();
  }

  void Server::start() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server, &config) == ESP_OK) {
      ESP_LOGI(TAG, "started server on port: %d", config.server_port);
      started = true;
      httpd_register_uri_handler(server, &ws_uri_config);
    } else {
      ESP_LOGE(TAG, "failed to start server");
    }
  }

  void Server::stop() {
    if (!started) return;

    if (httpd_stop(server) != ESP_OK) {
      ESP_LOGE(TAG, "failed to stop server");
    } else {
      started = false;
    }
  }

  esp_err_t Server::http_get_handler(httpd_req_t* req) {
    if (req->method == HTTP_GET) {
      ESP_LOGI(TAG, "handshake complete");
      return ESP_OK;
    }

    std::array<uint8_t, payload_size> payload;
    httpd_ws_frame_t ws_frame;
    memset(&ws_frame, 0, sizeof(httpd_ws_frame_t));
    ws_frame.payload = payload.data();
    auto error = httpd_ws_recv_frame(req, &ws_frame, payload.size());
    if (error != ESP_OK) {
      ESP_LOGE(TAG, "httpd_ws_recv_frame failed: %d", error);
      return error;
    }

    if (ws_frame.type == HTTPD_WS_TYPE_TEXT) {
      ESP_LOGI(TAG, "text frame received: %s", payload.data());
    } else {
      ESP_LOGI(TAG, "recieved frame with type: %d", ws_frame.type);
    }

    return ESP_OK;
  }

}