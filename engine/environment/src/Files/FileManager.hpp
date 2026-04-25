#pragma once
#include <filesystem>

#include "IFileManager.hpp"

namespace Engine::Environment::Files {
    class FileManager : public IFileManager {
    public:
        FileManager();

        ~FileManager() override = default;

        bool WriteTextToFile(std::string relative_path, std::string file_name, std::string content) override;

        Result<std::string> ReadTextFromFile(const std::string& relative_path) override;

        Result<std::string> ReadTextFromResourceFile(const std::string& relative_path) override;

        Result<FileBinary> ReadBinaryFromResourceFile(const std::string& relative_path) override;

        ResultCollection<FilePath> FindResourceFilesOfTypes(
                const std::string& directory, const std::vector<std::string>& file_types) override;

    private:
        std::string m_root_data_path;
        const std::string m_resource_dir_name = "resources";

        bool ValidatePath(const std::string& relative_path, std::filesystem::path& path, std::string& error,
                          std::error_code& error_code);

        template<typename T>
        Result<T> Success(T value);

        template<typename T>
        Result<T> Fail(const std::string& msg, int code);

        template<typename T>
        Result<T> FileNotFound();

        template<class T>
        ResultCollection<T> InvalidFileExtension(const std::string& msg);
    };
} // namespace
