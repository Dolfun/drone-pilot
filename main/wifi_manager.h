#ifndef WIFI_MANAGER_H_INCLUDED
#define WIFI_MANAGER_H_INCLUDED

#include "wifi_observer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event.h"

#define WIFI_LOG_TAG "wifi_manager"

class WifiManager {
public:
    static WifiManager& instance() {
        static WifiManager wifi_manager;
        return wifi_manager;
    }

    WifiManager(WifiManager&) = delete;
    WifiManager& operator=(const WifiManager&) = delete;
    WifiManager(WifiManager&&) = delete;
    WifiManager& operator=(WifiManager&&) = delete;

    static bool connected() { return is_connected; }
private:
    static bool is_connected;

    WifiManager();

    static void wifi_start_callback(void*, esp_event_base_t, int32_t, void*);
    static void ip_get_callback(void*, esp_event_base_t, int32_t, void*);
    static void wifi_disconnect_callback(void*, esp_event_base_t, int32_t, void*);
};

#endif