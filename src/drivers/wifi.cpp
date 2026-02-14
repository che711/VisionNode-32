#include "wifi.h"
#include "../utils/logger.h"
#include "config.h"

const char* WiFiDriver::TAG = "WIFI_DRV";

WiFiDriver& WiFiDriver::getInstance() {
    static WiFiDriver instance;
    return instance;
}

WiFiDriver::WiFiDriver()
    : _connected(false)
    , _lastCheckTime(0)
{
}

Result WiFiDriver::connect(const char* ssid, const char* password) {
    LOG_TAG_INFO(TAG, "Connecting to WiFi: %s", ssid);
    
    _ssid = ssid;
    _password = password;
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > WIFI_MAX_RETRIES * WIFI_RETRY_DELAY) {
            LOG_TAG_ERROR(TAG, "Connection timeout");
            return Result::ERROR_WIFI_CONNECT;
        }
        delay(WIFI_RETRY_DELAY);
        Serial.print(".");
    }
    
    _connected = true;
    LOG_TAG_INFO(TAG, "Connected! IP: %s", WiFi.localIP().toString().c_str());
    LOG_TAG_INFO(TAG, "RSSI: %d dBm", WiFi.RSSI());
    
    return Result::SUCCESS;
}

void WiFiDriver::disconnect() {
    WiFi.disconnect();
    _connected = false;
    LOG_TAG_INFO(TAG, "Disconnected");
}

bool WiFiDriver::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

WiFiInfo WiFiDriver::getInfo() const {
    WiFiInfo info;
    info.connected = isConnected();
    info.ssid = WiFi.SSID();
    info.ip = WiFi.localIP().toString();
    info.rssi = WiFi.RSSI();
    WiFi.macAddress(const_cast<uint8_t*>(info.mac));
    return info;
}

int32_t WiFiDriver::getRSSI() const {
    return WiFi.RSSI();
}

void WiFiDriver::handle() {
    // Auto-reconnect check
    if (millis() - _lastCheckTime > 10000) {
        if (!isConnected() && !_ssid.isEmpty()) {
            LOG_TAG_WARNING(TAG, "Connection lost, reconnecting...");
            connect(_ssid.c_str(), _password.c_str());
        }
        _lastCheckTime = millis();
    }
}
