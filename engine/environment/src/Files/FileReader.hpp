#pragma once
#include <__filesystem/filesystem_error.h>

#include "IFileReader.hpp"

namespace Engine::Environment::Files {
    class FileReader : public IFileReader {
    public:
        FileReader();

        ~FileReader() override = default;

        Result<std::string> ReadTextFromFile(const std::string& relative_path) override;

        Result<FileBinary> ReadBinaryFromFile(const std::string& relative_path) override;

    private:
        std::string m_root_data_path;
        const std::string m_resource_dir_name = "resources";

        bool ValidatePath(const std::string& relative_path, std::filesystem::path& path, std::string& error,
                          std::error_code& error_code);

        template<typename T>
        Result<T> Fail(const std::string& msg, int code);
    };
} // namespace
