#pragma once
#include <exception>


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef _WIN32
#include <windows.h>
#endif
#include <filesystem>

std::filesystem::path GetExecutablePath(){
    constexpr unsigned long BUFFER_SIZE = MAX_PATH;
    char path[BUFFER_SIZE];

    unsigned long result = GetModuleFileNameA(nullptr, path, BUFFER_SIZE);

    if (result == 0) {
        throw std::exception(std::format("Failed to get executable path, error: {}", GetLastError()).c_str());
    }

    if (result >= BUFFER_SIZE) {
        throw std::exception("Executable path too long");
    }

    return std::filesystem::path(path).parent_path();
}
