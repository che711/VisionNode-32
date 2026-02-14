/**
 * EdgeVision32 - Advanced ESP32-CAM Firmware
 * 
 * Professional layered architecture with clean separation of concerns:
 * - Core: Application orchestration
 * - Services: Business logic
 * - Drivers: Hardware abstraction (HAL)
 * - Utils: Helper functions
 */

#include <Arduino.h>
#include "core/app.h"

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(1000);
    
    // Initialize application core
    Result result = App::getInstance().init();
    
    if (result != Result::SUCCESS) {
        Serial.println("ERROR: Application initialization failed!");
        Serial.printf("Error code: %d\n", static_cast<int>(result));
        // Could enter deep sleep or restart here
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("Application ready!");
}

void loop() {
    // Run application main loop
    App::getInstance().run();
    
    // Small delay to prevent watchdog timeout
    delay(10);
}
