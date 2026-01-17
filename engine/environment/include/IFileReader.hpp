#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Engine::Environment::Files {
    struct FileBinary {
        std::vector<uint8_t> data;
        size_t size = 0;
        std::string name;
        std::string path;
        uint64_t mtime = 0;
    };

    struct LoadError {
        std::string message;
        int code = 0;
    };

    template<typename T>
    struct Result {
        T value;
        LoadError error;
        [[nodiscard]] bool Ok() const { return error.message.empty(); }
    };

    class IFileReader {
    public:
        virtual ~IFileReader() = default;

        virtual Result<std::string> ReadTextFromFile(const std::string& relative_path) = 0;

        virtual Result<FileBinary> ReadBinaryFromFile(const std::string& relative_path) = 0;
    };
}
