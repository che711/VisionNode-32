#include "camera.h"
#include "../utils/logger.h"
#include "config.h"

const char* CameraDriver::TAG = "CAM_DRV";

CameraDriver& CameraDriver::getInstance() {
    static CameraDriver instance;
    return instance;
}

CameraDriver::CameraDriver()
    : _initialized(false)
    , _sensor(nullptr)
{
    memset(&_config, 0, sizeof(_config));
    memset(&_stats, 0, sizeof(_stats));
}

Result CameraDriver::init(const CameraConfig& config) {
    LOG_TAG_INFO(TAG, "Initializing camera hardware...");
    
    camera_config_t cam_config;
    memset(&cam_config, 0, sizeof(cam_config));
    
    // Configure pins
    Result result = configurePins(cam_config);
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "Pin configuration failed");
        return result;
    }
    
    // Configure settings
    result = configureSettings(cam_config);
    if (result != Result::SUCCESS) {
        LOG_TAG_ERROR(TAG, "Settings configuration failed");
        return result;
    }
    
    // Apply user config
    cam_config.frame_size = (framesize_t)config.framesize;
    cam_config.jpeg_quality = config.quality;
    cam_config.fb_count = config.fb_count;
    
    // Initialize camera
    esp_err_t err = esp_camera_init(&cam_config);
    if (err != ESP_OK) {
        LOG_TAG_ERROR(TAG, "Camera init failed: 0x%x", err);
        return Result::ERROR_CAMERA_INIT;
    }
    
    // Get sensor for advanced settings
    _sensor = esp_camera_sensor_get();
    if (!_sensor) {
        LOG_TAG_ERROR(TAG, "Failed to get sensor");
        return Result::ERROR_CAMERA_INIT;
    }
    
    // Apply user settings
    _sensor->set_brightness(_sensor, config.brightness);
    _sensor->set_contrast(_sensor, config.contrast);
    _sensor->set_saturation(_sensor, config.saturation);
    _sensor->set_vflip(_sensor, config.vflip);
    _sensor->set_hmirror(_sensor, config.hmirror);
    
    _config = config;
    _initialized = true;
    
    LOG_TAG_INFO(TAG, "Camera initialized successfully");
    return Result::SUCCESS;
}

Result CameraDriver::configurePins(camera_config_t& config) {
    config.pin_pwdn = Pins::CAM_PWDN;
    config.pin_reset = Pins::CAM_RESET;
    config.pin_xclk = Pins::CAM_XCLK;
    config.pin_sccb_sda = Pins::CAM_SIOD;  // Fixed: was pin_sscb_sda (deprecated)
    config.pin_sccb_scl = Pins::CAM_SIOC;  // Fixed: was pin_sscb_scl (deprecated)
    
    config.pin_d7 = Pins::CAM_Y9;
    config.pin_d6 = Pins::CAM_Y8;
    config.pin_d5 = Pins::CAM_Y7;
    config.pin_d4 = Pins::CAM_Y6;
    config.pin_d3 = Pins::CAM_Y5;
    config.pin_d2 = Pins::CAM_Y4;
    config.pin_d1 = Pins::CAM_Y3;
    config.pin_d0 = Pins::CAM_Y2;
    
    config.pin_vsync = Pins::CAM_VSYNC;
    config.pin_href = Pins::CAM_HREF;
    config.pin_pclk = Pins::CAM_PCLK;
    
    return Result::SUCCESS;
}

Result CameraDriver::configureSettings(camera_config_t& config) {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.xclk_freq_hz = CAMERA_XCLK_FREQ;
    config.pixel_format = PIXFORMAT_JPEG;
    
    if (psramFound()) {
        config.fb_location = CAMERA_FB_IN_PSRAM;
        LOG_TAG_INFO(TAG, "Using PSRAM for frame buffers");
    } else {
        config.fb_location = CAMERA_FB_IN_DRAM;
        LOG_TAG_WARNING(TAG, "PSRAM not found, using DRAM");
    }
    
    return Result::SUCCESS;
}

camera_fb_t* CameraDriver::captureFrame() {
    if (!_initialized) {
        LOG_TAG_ERROR(TAG, "Camera not initialized");
        return nullptr;
    }
    
    uint32_t startTime = millis();
    camera_fb_t* fb = esp_camera_fb_get();
    uint32_t captureTime = millis() - startTime;
    
    if (!fb) {
        LOG_TAG_ERROR(TAG, "Frame capture failed");
        _stats.errors++;
        return nullptr;
    }
    
    updateStats(fb, captureTime);
    return fb;
}

void CameraDriver::releaseFrame(camera_fb_t* fb) {
    if (fb) {
        esp_camera_fb_return(fb);
    }
}

void CameraDriver::updateStats(const camera_fb_t* fb, uint32_t captureTime) {
    _stats.frames_captured++;
    _stats.capture_time_ms = captureTime;
    _stats.jpeg_size_avg = (_stats.jpeg_size_avg * (_stats.frames_captured - 1) + fb->len) / _stats.frames_captured;
}

Result CameraDriver::setFrameSize(framesize_t size) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    
    int result = _sensor->set_framesize(_sensor, size);
    if (result == 0) {
        _config.framesize = size;
        LOG_TAG_INFO(TAG, "Frame size set to %d", size);
        return Result::SUCCESS;
    }
    return Result::ERROR_GENERIC;
}

Result CameraDriver::setQuality(int quality) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    
    int result = _sensor->set_quality(_sensor, quality);
    if (result == 0) {
        _config.quality = quality;
        return Result::SUCCESS;
    }
    return Result::ERROR_GENERIC;
}

Result CameraDriver::setBrightness(int brightness) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    return (_sensor->set_brightness(_sensor, brightness) == 0) ? Result::SUCCESS : Result::ERROR_GENERIC;
}

Result CameraDriver::setContrast(int contrast) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    return (_sensor->set_contrast(_sensor, contrast) == 0) ? Result::SUCCESS : Result::ERROR_GENERIC;
}

Result CameraDriver::setSaturation(int saturation) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    return (_sensor->set_saturation(_sensor, saturation) == 0) ? Result::SUCCESS : Result::ERROR_GENERIC;
}

Result CameraDriver::setVFlip(bool enable) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    return (_sensor->set_vflip(_sensor, enable ? 1 : 0) == 0) ? Result::SUCCESS : Result::ERROR_GENERIC;
}

Result CameraDriver::setHMirror(bool enable) {
    if (!_initialized || !_sensor) return Result::ERROR_NOT_INITIALIZED;
    return (_sensor->set_hmirror(_sensor, enable ? 1 : 0) == 0) ? Result::SUCCESS : Result::ERROR_GENERIC;
}

void CameraDriver::setFlash(bool on) {
    pinMode(Pins::FLASH_LED, OUTPUT);
    digitalWrite(Pins::FLASH_LED, on ? HIGH : LOW);
}

void CameraDriver::resetStats() {
    memset(&_stats, 0, sizeof(_stats));
}

void CameraDriver::deinit() {
    if (_initialized) {
        esp_camera_deinit();
        _initialized = false;
        _sensor = nullptr;
        LOG_TAG_INFO(TAG, "Camera deinitialized");
    }
}
