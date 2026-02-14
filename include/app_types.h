#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <Arduino.h>

// ============================================================
// Result Codes
// ============================================================
enum class Result {
    SUCCESS = 0,
    ERROR_GENERIC = -1,
    ERROR_CAMERA_INIT = -2,
    ERROR_WIFI_CONNECT = -3,
    ERROR_SD_CARD = -4,
    ERROR_MEMORY = -5,
    ERROR_TIMEOUT = -6,
    ERROR_INVALID_PARAM = -7,
    ERROR_NOT_INITIALIZED = -8
};

// ============================================================
// Application States
// ============================================================
enum class AppState {
    INITIALIZING,
    IDLE,
    RUNNING,
    ERROR,
    OTA_UPDATE,
    DEEP_SLEEP
};

// ============================================================
// Log Levels
// ============================================================
enum class LogLevel {
    NONE = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4,
    VERBOSE = 5
};

// ============================================================
// Hardware Pin Definitions (ESP32-CAM AI-Thinker)
// ============================================================
namespace Pins {
    // LEDs
    constexpr uint8_t LED_BUILTIN = 33;
    constexpr uint8_t FLASH_LED = 4;
    
    // Camera OV2640
    constexpr uint8_t CAM_PWDN = 32;
    constexpr uint8_t CAM_RESET = -1;
    constexpr uint8_t CAM_XCLK = 0;
    constexpr uint8_t CAM_SIOD = 26;
    constexpr uint8_t CAM_SIOC = 27;
    
    constexpr uint8_t CAM_Y9 = 35;
    constexpr uint8_t CAM_Y8 = 34;
    constexpr uint8_t CAM_Y7 = 39;
    constexpr uint8_t CAM_Y6 = 36;
    constexpr uint8_t CAM_Y5 = 21;
    constexpr uint8_t CAM_Y4 = 19;
    constexpr uint8_t CAM_Y3 = 18;
    constexpr uint8_t CAM_Y2 = 5;
    
    constexpr uint8_t CAM_VSYNC = 25;
    constexpr uint8_t CAM_HREF = 23;
    constexpr uint8_t CAM_PCLK = 22;
}

// ============================================================
// System Limits and Thresholds
// ============================================================
namespace Limits {
    constexpr float TEMP_WARNING = 65.0f;      // °C
    constexpr float TEMP_CRITICAL = 75.0f;     // °C
    constexpr uint32_t RAM_WARNING = 50000;    // bytes
    constexpr uint32_t RAM_CRITICAL = 30000;   // bytes
    constexpr uint8_t MAX_CLIENTS = 5;
}

// ============================================================
// Timeouts
// ============================================================
namespace Timeouts {
    constexpr uint32_t WIFI_CONNECT = 20000;    // ms
    constexpr uint32_t CAMERA_INIT = 10000;     // ms
    constexpr uint32_t HTTP_REQUEST = 10000;    // ms
}

// ============================================================
// Data Structures
// ============================================================

// Camera configuration
struct CameraConfig {
    int framesize;          // FRAMESIZE_*
    int quality;            // 10-63
    int brightness;         // -2 to 2
    int contrast;           // -2 to 2
    int saturation;         // -2 to 2
    bool vflip;
    bool hmirror;
    int fb_count;           // 1 or 2
};

// System statistics
struct SystemStats {
    uint32_t uptime_sec;
    uint32_t free_heap;
    uint32_t free_psram;
    float temperature;
    int32_t wifi_rssi;
    AppState state;
};

// Camera statistics  
struct CameraStats {
    uint32_t frames_captured;
    uint32_t frames_dropped;
    uint32_t jpeg_size_avg;
    uint16_t capture_time_ms;
    uint32_t errors;
};

// WiFi information
struct WiFiInfo {
    bool connected;
    String ssid;
    String ip;
    int32_t rssi;
    uint8_t mac[6];
};

// Motion detection config
struct MotionConfig {
    bool enabled;
    uint8_t threshold;          // 0-100
    uint32_t cooldown_ms;
    bool capture_on_detection;
};

// ============================================================
// Callback Types
// ============================================================
typedef std::function<void(const char* message)> ErrorCallback;
typedef std::function<void(void)> MotionCallback;
typedef std::function<void(uint8_t progress)> ProgressCallback;

#endif // APP_TYPES_H
