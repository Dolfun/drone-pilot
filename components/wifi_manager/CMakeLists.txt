idf_component_register(
  SRCS "WifiManager.h" "WifiManager.cc"
  INCLUDE_DIRS "."
  REQUIRES nvs_flash esp_wifi
)