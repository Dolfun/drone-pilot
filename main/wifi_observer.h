#ifndef WIFI_OBSERVER_H_INCLUDED
#define WIFI_OBSERVER_H_INCLUDED

#include "wifi_manager.h"
#include <forward_list>

class WifiObserver {
public:
    WifiObserver() {
        it = (observers.empty() ? observers.before_begin() :
                std::prev(observers.end()));
        observers.emplace_front(this);
    }

    virtual ~WifiObserver() {
        observers.erase_after(it);
    }

    WifiObserver(WifiObserver&) = delete;
    WifiObserver& operator=(const WifiObserver&) = delete;
    WifiObserver(WifiObserver&&) = default;
    WifiObserver& operator=(WifiObserver&&) = default;

    virtual void on_connect() = 0;
    virtual void on_disconnect() = 0;
private:
    friend class WifiManager;
    static std::forward_list<WifiObserver*> observers;
    std::forward_list<WifiObserver*>::iterator it;
};

#endif