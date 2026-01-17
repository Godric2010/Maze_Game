#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
#elif defined(__APPLE__)
    #include <vector>
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <vector>
#else
    #error Unsupported platform
#endif

namespace Engine::Environment::Files {
    inline std::filesystem::path GetExecutablePath() {
        #if defined(_WIN32)
        std::wstring buffer;
        buffer.resize(512);

        while (true) {
            const DWORD len = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
            if (len == 0) {
                throw std::runtime_error("GetModuleFileNameW failed");
            }
            if (len < buffer.size() - 1) {
                buffer.resize(len);
                break;
            }

            // Buffer was too small
            buffer.resize(buffer.size() * 2);
        }
        return std::filesystem::path(buffer);

        #elif defined(__APPLE__)

        uint32_t size = 0;
        _NSGetExecutablePath(nullptr, &size);
        std::vector<char> buffer(size);
        if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
            throw std::runtime_error("_NSGetExecutablePath failed");
        }
        const auto string = std::string(buffer.data());
        return std::filesystem::path(string);

        #elif defined(__linux__)
        std::vector<char> buffer(512);
        while (true) {
            const ssize_t len = ::readlink("/proc/self/exe", buffer.data(), buffer.size() - 1);
            if (len == 0) {
                throw std::runtime_error("readlink /proc/self/exe failed");
            }
            if (static_cast<size_t>(len) < buffer.size() - 1) {
                buffer[len] = '\0';
                return std::filesystem::path(std::string(buffer.data()));
            }
            // Buffer was too small
            buffer.resize(buffer.size() * 2);
        }
        #endif
    }

    inline std::filesystem::path GetExecutableDirectory() {
        auto executable_path = GetExecutablePath();

        std::error_code err;
        executable_path = weakly_canonical(executable_path, err);

        return executable_path.parent_path();
    }
}
