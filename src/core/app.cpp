#include "app.h"
#include "../utils/logger.h"
#include "../drivers/camera.h"
#include "../drivers/wifi.h"
#include "config.h"
#include "version.h"

const char* App::TAG = "APP";

App& App::getInstance() {
    static App instance;
    return instance;
}

App::App()
    : _state(AppState::INITIALIZING)
    , _startTime(0)
    , _lastMonitorTime(0)
{
}

Result App::init() {
    LOG_TAG_INFO(TAG, "===========================================");
    LOG_TAG_INFO(TAG, "%s v%s", PROJECT_NAME, PROJECT_VERSION);
    LOG_TAG_INFO(TAG, "Build: %s %s", BUILD_DATE, BUILD_TIME);
    LOG_TAG_INFO(TAG, "===========================================");
    
    _startTime = millis();
    _state = AppState::INITIALIZING;
    
    // Initialize logger
    Logger::getInstance().begin(LOG_LEVEL, LOG_TO_SERIAL, LOG_TO_SD);
    
    // Initialize drivers (HAL layer)
    Result result = initDrivers();
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "Driver initialization failed");
        _state = AppState::ERROR;
        return result;
    }
    
    // Initialize services (business logic layer)
    result = initServices();
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "Service initialization failed");
        _state = AppState::ERROR;
        return result;
    }
    
    _state = AppState::RUNNING;
    LOG_TAG_INFO(TAG, "Application initialized successfully!");
    LOG_TAG_INFO(TAG, "Free heap: %u bytes", ESP.getFreeHeap());
    
    return Result::SUCCESS;
}

Result App::initDrivers() {
    LOG_TAG_INFO(TAG, "Initializing drivers...");
    Result result;
    
    // WiFi driver
    result = WiFiDriver::getInstance().connect(WIFI_SSID, WIFI_PASSWORD);
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "WiFi connection failed");
        return result;
    }
    
    // Camera driver
    #if CAMERA_ENABLED
    CameraConfig camConfig = {
        .framesize = DEFAULT_FRAME_SIZE,
        .quality = DEFAULT_JPEG_QUALITY,
        .brightness = 0,
        .contrast = 0,
        .saturation = 0,
        .vflip = false,
        .hmirror = false,
        .fb_count = DEFAULT_FB_COUNT
    };
    
    result = CameraDriver::getInstance().init(camConfig);
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "Camera initialization failed");
        return result;
    }
    #endif
    
    LOG_TAG_INFO(TAG, "Drivers initialized");
    return Result::SUCCESS;
}

Result App::initServices() {
    LOG_TAG_INFO(TAG, "Initializing services...");
    
    // TODO: Initialize motion detection service
    // TODO: Initialize upload service
    // TODO: Initialize OTA service
    // TODO: Initialize web server
    
    LOG_TAG_INFO(TAG, "Services initialized");
    return Result::SUCCESS;
}

void App::run() {
    // Handle periodic tasks
    WiFiDriver::getInstance().handle();
    
    // System monitoring
    if (millis() - _lastMonitorTime > STATS_UPDATE_INTERVAL) {
        monitorSystem();
        _lastMonitorTime = millis();
    }
}

void App::monitorSystem() {
    #if TEMP_MONITORING_ENABLED
    float temp = temperatureRead();
    if (temp > Limits::TEMP_WARNING) {
        LOG_TAG_WARNING(TAG, "High temperature: %.1f°C", temp);
    }
    #endif
    
    #if MEMORY_MONITORING_ENABLED
    uint32_t freeHeap = ESP.getFreeHeap();
    if (freeHeap < Limits::RAM_WARNING) {
        LOG_TAG_WARNING(TAG, "Low memory: %u bytes", freeHeap);
    }
    #endif
}

SystemStats App::getStats() const {
    SystemStats stats;
    stats.uptime_sec = (millis() - _startTime) / 1000;
    stats.free_heap = ESP.getFreeHeap();
    stats.free_psram = ESP.getFreePsram();
    stats.temperature = temperatureRead();
    stats.wifi_rssi = WiFiDriver::getInstance().getRSSI();
    stats.state = _state;
    return stats;
}
