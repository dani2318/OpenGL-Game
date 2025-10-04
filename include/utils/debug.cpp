#include "debug.hpp"
#include <iostream>

namespace Debug {
    /*
        for(int i = 0; i < OutputDevicesCount; i++){
            if (logLevel < g_OutputDevices[i].logLevel)
                continue;

            if (g_OutputDevices[i].colored)
                g_OutputDevices[i].device->Write(g_LogSeverityColors[static_cast<int>(logLevel)]);

            g_OutputDevices[i].device->Format("[%s] ", module);
            g_OutputDevices[i].device->VFormat(fmt, args);

            if (g_OutputDevices[i].colored)
                g_OutputDevices[i].device->Write(g_ColorReset);

            g_OutputDevices[i].device->Write('\n');
        }
     */
     static void Log(const char* module, Debug::Level log_level, const char* fmt, va_list args) {
         char buffer[1024];
         vsnprintf(buffer, sizeof(buffer), fmt, args);

         std::cout << G_LOG_SEVERITY_COLORS[static_cast<int>(log_level)];
         std::cout << "[" << module << "] " << buffer << G_COLOR_RESET << "\n";
     }

    void Debug(const char* module, const char* fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);
        Log(module, Debug::Level::DEBUG, fmt, args);
        va_end(args);
    }

    void Info(const char* module, const char* fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);
        Log(module, Debug::Level::INFO, fmt, args);
        va_end(args);
    }

    void Warn(const char* module, const char* fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);
        Log(module, Debug::Level::WARN, fmt, args);
        va_end(args);
    }

    void Error(const char* module, const char* fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);
        Log(module, Debug::Level::ERR, fmt, args);
        va_end(args);
    }

    void Critical(const char* module, const char* fmt, ...) {
        va_list args = nullptr;
        va_start(args, fmt);
        Log(module, Debug::Level::CRITICAL, fmt, args);
        va_end(args);
    }
}
