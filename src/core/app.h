#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "app_types.h"

/**
 * @brief Application Core - Main Orchestrator
 * 
 * Coordinates all drivers and services.
 * Manages application lifecycle and state.
 * 
 * Layer: CORE (Application)
 * Dependencies: drivers, services
 */
class App {
public:
    static App& getInstance();
    
    Result init();
    void run();
    
    AppState getState() const { return _state; }
    SystemStats getStats() const;

private:
    App();
    ~App() = default;
    
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    
    Result initDrivers();
    Result initServices();
    void monitorSystem();
    
    AppState _state;
    uint32_t _startTime;
    uint32_t _lastMonitorTime;
    
    static const char* TAG;
};

#endif // APP_H
