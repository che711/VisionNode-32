#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "app_types.h"

/**
 * @brief Centralized logging system with multiple output targets
 * 
 * Features:
 * - Multiple log levels (ERROR, WARNING, INFO, DEBUG, VERBOSE)
 * - Colored output for Serial terminal
 * - Optional SD card logging
 * - Module tags for filtering
 * - Thread-safe operations
 */
class Logger {
public:
    /**
     * @brief Get singleton instance
     */
    static Logger& getInstance();
    
    /**
     * @brief Initialize logger
     * @param level Minimum log level to display
     * @param useSerial Enable Serial output
     * @param useSD Enable SD card logging
     */
    void begin(LogLevel level = LogLevel::INFO, bool useSerial = true, bool useSD = false);
    
    /**
     * @brief Set minimum log level
     */
    void setLevel(LogLevel level);
    
    /**
     * @brief Get current log level
     */
    LogLevel getLevel() const { return _currentLevel; }
    
    /**
     * @brief Log methods for different levels
     */
    void error(const char* format, ...);
    void warning(const char* format, ...);
    void info(const char* format, ...);
    void debug(const char* format, ...);
    void verbose(const char* format, ...);
    
    /**
     * @brief Log with module tag
     * @param level Log level
     * @param tag Module identifier (e.g., "CAMERA", "WIFI")
     * @param format Printf-style format string
     */
    void log(LogLevel level, const char* tag, const char* format, ...);
    
private:
    Logger();
    ~Logger() = default;
    
    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void writeLog(LogLevel level, const char* tag, const char* message);
    const char* getLevelString(LogLevel level);
    const char* getLevelColor(LogLevel level);
    
    LogLevel _currentLevel;
    bool _serialEnabled;
    bool _sdEnabled;
    String _logFilePath;
    
    static const int BUFFER_SIZE = 512;
};

// Convenience macros
#define LOG_ERROR(fmt, ...)   Logger::getInstance().error(fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) Logger::getInstance().warning(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    Logger::getInstance().info(fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)   Logger::getInstance().debug(fmt, ##__VA_ARGS__)
#define LOG_VERBOSE(fmt, ...) Logger::getInstance().verbose(fmt, ##__VA_ARGS__)

// Tagged logging macros
#define LOG_TAG_ERROR(tag, fmt, ...)   Logger::getInstance().log(LogLevel::ERROR, tag, fmt, ##__VA_ARGS__)
#define LOG_TAG_WARNING(tag, fmt, ...) Logger::getInstance().log(LogLevel::WARNING, tag, fmt, ##__VA_ARGS__)
#define LOG_TAG_INFO(tag, fmt, ...)    Logger::getInstance().log(LogLevel::INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_TAG_DEBUG(tag, fmt, ...)   Logger::getInstance().log(LogLevel::DEBUG, tag, fmt, ##__VA_ARGS__)

#endif // LOGGER_H
