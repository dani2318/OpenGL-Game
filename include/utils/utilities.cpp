#include "utilities.hpp"


std::filesystem::path GetExecutablePath() {
#ifdef _WIN32
    std::vector<char> path(MAX_PATH);
    DWORD result;

    while (true) {
        result = GetModuleFileNameA(nullptr, path.data(), static_cast<DWORD>(path.size()));

        if (result == 0) {
            throw std::runtime_error(std::format("Failed to get executable path, error: {}", GetLastError()));
        }

        if (result < path.size()) {
            break;
        }

        path.resize(path.size() * 2);
    }

    return std::filesystem::path(path.data());
#else
    std::vector<char> path(PATH_MAX);
    ssize_t count = readlink("/proc/self/exe", path.data(), path.size());

    if (count == -1) {
        throw std::runtime_error(std::format("Failed to get executable path, errno: {}", errno));
    }

    if (count >= static_cast<ssize_t>(path.size())) {
        throw std::runtime_error("Executable path too long");
    }

    // readlink doesn't null-terminate, so we need to do it ourselves
    path[count] = '\0';

    return std::filesystem::path(path.data());
#endif
}
