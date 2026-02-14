#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include <Arduino.h>
#include <WiFi.h>
#include "app_types.h"

/**
 * @brief WiFi Hardware Abstraction Layer
 * 
 * Low-level driver for WiFi connectivity.
 * Handles connection, reconnection, and signal monitoring.
 * 
 * Layer: DRIVER (HAL)
 * Dependencies: Hardware only (WiFi.h)
 */
class WiFiDriver {
public:
    static WiFiDriver& getInstance();
    
    Result connect(const char* ssid, const char* password);
    void disconnect();
    
    bool isConnected() const;
    WiFiInfo getInfo() const;
    int32_t getRSSI() const;
    
    void handle();  // Called in loop for auto-reconnect

private:
    WiFiDriver();
    ~WiFiDriver() = default;
    
    WiFiDriver(const WiFiDriver&) = delete;
    WiFiDriver& operator=(const WiFiDriver&) = delete;
    
    bool _connected;
    String _ssid;
    String _password;
    uint32_t _lastCheckTime;
    
    static const char* TAG;
};

#endif // WIFI_DRIVER_H
