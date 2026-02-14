#ifndef VERSION_H
#define VERSION_H

// Project information
#define PROJECT_NAME "EdgeVision32"
#define PROJECT_VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

// Git information (populated by build script)
#ifndef GIT_COMMIT
#define GIT_COMMIT "unknown"
#endif

#ifndef GIT_BRANCH
#define GIT_BRANCH "main"
#endif

// Component versions
#define CAMERA_DRIVER_VERSION "1.0.0"
#define WIFI_DRIVER_VERSION "1.0.0"
#define MOTION_SERVICE_VERSION "1.0.0"

// Build configuration
#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif

#endif // VERSION_H
