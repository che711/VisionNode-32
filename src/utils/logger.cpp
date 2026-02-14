#include "logger.h"
#include "SD_MMC.h"
#include <stdarg.h>

// ANSI colors
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_GRAY    "\x1b[90m"
#define COLOR_RESET   "\x1b[0m"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : _currentLevel(LogLevel::INFO)
    , _serialEnabled(true)
    , _sdEnabled(false)
    , _logFilePath("/logs/edgevision.log")
{
}

void Logger::begin(LogLevel level, bool useSerial, bool useSD) {
    _currentLevel = level;
    _serialEnabled = useSerial;
    _sdEnabled = useSD;
    
    if (_serialEnabled && !Serial) {
        Serial.begin(115200);
        delay(100);
    }
    
    info("Logger initialized at level %d", static_cast<int>(level));
}

void Logger::setLevel(LogLevel level) {
    _currentLevel = level;
}

void Logger::error(const char* format, ...) {
    if (_currentLevel < LogLevel::ERROR) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LogLevel::ERROR, nullptr, buffer);
}

void Logger::warning(const char* format, ...) {
    if (_currentLevel < LogLevel::WARNING) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LogLevel::WARNING, nullptr, buffer);
}

void Logger::info(const char* format, ...) {
    if (_currentLevel < LogLevel::INFO) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LogLevel::INFO, nullptr, buffer);
}

void Logger::debug(const char* format, ...) {
    if (_currentLevel < LogLevel::DEBUG) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LogLevel::DEBUG, nullptr, buffer);
}

void Logger::verbose(const char* format, ...) {
    if (_currentLevel < LogLevel::VERBOSE) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(LogLevel::VERBOSE, nullptr, buffer);
}

void Logger::log(LogLevel level, const char* tag, const char* format, ...) {
    if (_currentLevel < level) return;
    
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    writeLog(level, tag, buffer);
}

void Logger::writeLog(LogLevel level, const char* tag, const char* message) {
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%010lu", millis());
    
    const char* levelStr = getLevelString(level);
    const char* color = getLevelColor(level);
    
    // Serial output with color
    if (_serialEnabled) {
        Serial.print(color);
        Serial.printf("[%s][%s]", timestamp, levelStr);
        if (tag) {
            Serial.printf("[%s]", tag);
        }
        Serial.printf(" %s", message);
        Serial.println(COLOR_RESET);
    }
    
    // SD card output (without color)
    if (_sdEnabled && SD_MMC.cardSize() > 0) {
        File logFile = SD_MMC.open(_logFilePath.c_str(), FILE_APPEND);
        if (logFile) {
            logFile.printf("[%s][%s]", timestamp, levelStr);
            if (tag) {
                logFile.printf("[%s]", tag);
            }
            logFile.printf(" %s\n", message);
            logFile.close();
        }
    }
}

const char* Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::INFO:    return "INFO ";
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::VERBOSE: return "VERB ";
        default:                return "?????";
    }
}

const char* Logger::getLevelColor(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR:   return COLOR_RED;
        case LogLevel::WARNING: return COLOR_YELLOW;
        case LogLevel::INFO:    return COLOR_GREEN;
        case LogLevel::DEBUG:   return COLOR_CYAN;
        case LogLevel::VERBOSE: return COLOR_GRAY;
        default:                return COLOR_RESET;
    }
}
