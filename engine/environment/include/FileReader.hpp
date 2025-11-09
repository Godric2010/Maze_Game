#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <optional>

namespace Engine::Environment {
    class FileReader {
    public:
        static std::optional<std::string> ReadTextContentFromFile(const std::string &filepath) {
            const std::filesystem::path resource_path = std::filesystem::current_path() / "resources" / filepath;

            std::ifstream file(resource_path, std::ios::ate | std::ios::binary);
            if (!file.is_open()) {
                return std::nullopt;
            }
            const size_t file_size = (size_t) file.tellg();
            std::vector<char> buffer(file_size);
            file.seekg(0);
            file.read(buffer.data(), file_size);
            file.close();
            return std::make_optional(std::string{buffer.data(), file_size});
        }

    private:
    };
} // namespace
