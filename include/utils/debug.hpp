#include <cstdarg>

#define MIN_LOG_LEVEL Debug::Level::INFO

namespace Debug {

    static const char* const G_LOG_SEVERITY_COLORS[] =
    {
        "\033[2;37m",
        "\033[37m",
        "\033[1;33m",
        "\033[1;31m",
        "\033[1;37;41m",
    };

    static const char* const G_COLOR_RESET = "\033[0m";

    enum class Level {
        DEBUG = 0,
        INFO  = 1,
        WARN = 2,
        ERR = 3,
        CRITICAL = 4
    };

    void Debug(const char* module, const char* fmt, ...);
    void Info(const char* module, const char* fmt, ...);
    void Warn(const char* module, const char* fmt, ...);
    void Error(const char* module, const char* fmt, ...);
    void Critical(const char* module, const char* fmt, ...);

}
