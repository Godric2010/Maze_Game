#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Engine::Environment::Files
{
    struct FileBinary
    {
        std::vector<uint8_t> data;
        size_t size = 0;
        std::string name;
        std::string path;
        uint64_t mtime = 0;
    };

    struct LoadError
    {
        std::string message;
        int code = 0;
    };

    enum class ResultType
    {
        Ok,
        FileNotFound,
        FileError,
    };

    template <typename T>
    struct Result
    {
        ResultType type;
        T value;
        LoadError error;

        [[nodiscard]] bool Ok() const
        {
            return type == ResultType::Ok && error.message.empty();
        }
    };

    class IFileManager
    {
        public:
            virtual ~IFileManager() = default;

            virtual bool WriteTextToFile(std::string file_path, std::string file_name, std::string content) = 0;
            
            virtual Result<std::string> ReadTextFromFile(const std::string& file_name) = 0;

            virtual Result<std::string> ReadTextFromResourceFile(const std::string& relative_path) = 0;

            virtual Result<FileBinary> ReadBinaryFromResourceFile(const std::string& relative_path) = 0;

            virtual std::vector<std::string> FindResourceFilesOfType(const std::string& directory,
                                                                     const std::string& file_type) = 0;
    };
}
