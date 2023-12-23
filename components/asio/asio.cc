#include <cstdlib>
#include "esp_log.h"

namespace asio::detail {
  template <typename Exception>
  void throw_exception(const Exception& e) {
    ESP_LOGE("asio", "exception occured: %s", e.what());
    std::abort();
  }
};

#include "asio/impl/src.hpp"