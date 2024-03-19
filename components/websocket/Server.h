#pragma once
#include "Wifi.h"
#include <esp_http_server.h>
#include <memory>
#include <atomic>

namespace Websocket {

  class Server : public Wifi::Interface {
  public:
  Server() : ws_uri_config {
    .uri = "/drone",
    .method = HTTP_GET,
    .handler = http_get_handler,
    .user_ctx = nullptr,
    .is_websocket = true,
  } {};

  void on_wifi_connect() override;
  void on_wifi_disconnect() override;

  private:
    void start();
    void stop();

    static esp_err_t http_get_handler(httpd_req_t*);

    const httpd_uri_t ws_uri_config;
    std::atomic<bool> started;
    httpd_handle_t server;
  };

}