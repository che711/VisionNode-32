#ifndef CONFIG_H
#define CONFIG_H

#include "app_types.h"

// ============================================================
// WiFi Configuration
// ============================================================
#define WIFI_SSID "SkyNet"
#define WIFI_PASSWORD "password"

// Backup WiFi (optional)
#define WIFI_SSID_BACKUP ""
#define WIFI_PASSWORD_BACKUP ""

#define WIFI_MAX_RETRIES 40
#define WIFI_RETRY_DELAY 500  // ms

// ============================================================
// Web Server Configuration
// ============================================================
#define HTTP_PORT 80
#define STREAM_PORT 81

// ============================================================
// Camera Configuration  
// ============================================================
#define CAMERA_ENABLED true
#define DEFAULT_FRAME_SIZE FRAMESIZE_XGA  // 1024x768
#define DEFAULT_JPEG_QUALITY 10           // 10=best, 63=worst
#define DEFAULT_FB_COUNT 2

// Camera timing
#define CAMERA_XCLK_FREQ 20000000  // 20MHz

// ============================================================
// SD Card Configuration
// ============================================================
#define SD_CARD_ENABLED true
#define SD_MOUNT_POINT "/sdcard"
#define PHOTO_DIR "/photos"
#define VIDEO_DIR "/videos"

// ============================================================
// Motion Detection Configuration
// ============================================================
#define MOTION_ENABLED false
#define MOTION_THRESHOLD 20               // 0-100
#define MOTION_COOLDOWN 5000              // ms
#define MOTION_AUTO_CAPTURE true

// ============================================================
// Upload Service Configuration
// ============================================================
#define UPLOAD_ENABLED false
#define UPLOAD_SERVER_URL "https://your-server.com/api/upload"
#define UPLOAD_API_KEY ""
#define UPLOAD_TIMEOUT 30000  // ms

// ============================================================
// OTA Configuration
// ============================================================
#define OTA_ENABLED true
#define OTA_HOSTNAME "edgevision32"
#define OTA_PASSWORD "edgevision32"
#define OTA_PORT 3232

// ============================================================
// Logging Configuration
// ============================================================
#define LOG_LEVEL LogLevel::INFO
#define LOG_TO_SERIAL true
#define LOG_TO_SD false
#define SERIAL_BAUD_RATE 115200

// ============================================================
// System Monitoring
// ============================================================
#define TEMP_MONITORING_ENABLED true
#define TEMP_CHECK_INTERVAL 10000  // ms

#define MEMORY_MONITORING_ENABLED true
#define MEMORY_CHECK_INTERVAL 5000  // ms

#define WATCHDOG_ENABLED true
#define WATCHDOG_TIMEOUT 30000  // ms

// ============================================================
// Performance Settings
// ============================================================
#define STATS_UPDATE_INTERVAL 2000  // ms
#define MAX_CONCURRENT_CLIENTS 5

// Task priorities (higher = more priority)
#define CAMERA_TASK_PRIORITY 5
#define NETWORK_TASK_PRIORITY 4
#define SERVICE_TASK_PRIORITY 3
#define MONITOR_TASK_PRIORITY 2

// Task stack sizes
#define CAMERA_TASK_STACK 8192
#define NETWORK_TASK_STACK 8192
#define SERVICE_TASK_STACK 4096
#define MONITOR_TASK_STACK 2048

// ============================================================
// Feature Flags
// ============================================================
#define FEATURE_WEB_SERVER true
#define FEATURE_STREAM true
#define FEATURE_MOTION_DETECTION false
#define FEATURE_UPLOAD false
#define FEATURE_OTA true
#define FEATURE_CONFIG_SERVER true

// ============================================================
// Debug Settings
// ============================================================
#ifdef DEBUG
    #define DEBUG_CAMERA true
    #define DEBUG_WIFI true
    #define DEBUG_SERVICES true
    #define VERBOSE_LOGGING true
#else
    #define DEBUG_CAMERA false
    #define DEBUG_WIFI false
    #define DEBUG_SERVICES false
    #define VERBOSE_LOGGING false
#endif

// ============================================================
// Advanced Settings
// ============================================================
#define ENABLE_PSRAM true
#define BROWNOUT_DETECTOR false  // Disable for stability
#define ENABLE_CORE_DUMP false

#endif // CONFIG_H
