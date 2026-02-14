#ifndef CAMERA_DRIVER_H
#define CAMERA_DRIVER_H

#include <Arduino.h>
#include "esp_camera.h"
#include "app_types.h"

/**
 * @brief Camera Hardware Abstraction Layer (HAL)
 * 
 * Low-level driver for OV2640 camera on ESP32-CAM.
 * Handles direct hardware interaction and frame buffer management.
 * 
 * Layer: DRIVER (HAL)
 * Dependencies: Hardware only (esp_camera.h)
 */
class CameraDriver {
public:
    /**
     * @brief Get singleton instance
     */
    static CameraDriver& getInstance();
    
    /**
     * @brief Initialize camera hardware
     * @param config Camera configuration
     * @return Result::SUCCESS or error code
     */
    Result init(const CameraConfig& config);
    
    /**
     * @brief Check if camera is initialized
     */
    bool isInitialized() const { return _initialized; }
    
    /**
     * @brief Capture a frame from camera
     * @return Pointer to frame buffer or nullptr on error
     * @note Caller must call releaseFrame() after use!
     */
    camera_fb_t* captureFrame();
    
    /**
     * @brief Release frame buffer back to driver
     * @param fb Frame buffer to release
     */
    void releaseFrame(camera_fb_t* fb);
    
    /**
     * @brief Set camera parameters
     */
    Result setFrameSize(framesize_t size);
    Result setQuality(int quality);
    Result setBrightness(int brightness);
    Result setContrast(int contrast);
    Result setSaturation(int saturation);
    Result setVFlip(bool enable);
    Result setHMirror(bool enable);
    
    /**
     * @brief Control flash LED
     */
    void setFlash(bool on);
    
    /**
     * @brief Get current configuration
     */
    CameraConfig getConfig() const { return _config; }
    
    /**
     * @brief Get camera statistics
     */
    CameraStats getStats() const { return _stats; }
    
    /**
     * @brief Reset statistics
     */
    void resetStats();
    
    /**
     * @brief Deinitialize camera
     */
    void deinit();

private:
    CameraDriver();
    ~CameraDriver() = default;
    
    // Prevent copying
    CameraDriver(const CameraDriver&) = delete;
    CameraDriver& operator=(const CameraDriver&) = delete;
    
    Result configurePins(camera_config_t& config);
    Result configureSettings(camera_config_t& config);
    void updateStats(const camera_fb_t* fb, uint32_t captureTime);
    
    bool _initialized;
    CameraConfig _config;
    CameraStats _stats;
    sensor_t* _sensor;
    
    static const char* TAG;
};

#endif // CAMERA_DRIVER_H
