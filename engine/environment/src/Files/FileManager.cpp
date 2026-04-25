#include "FileManager.hpp"

#include <algorithm>
#include <fstream>

#include "ExecutablePath.hpp"

namespace Engine::Environment::Files {
    FileManager::FileManager() {
        m_root_data_path = GetExecutableDirectory().string();
    }

    bool FileManager::WriteTextToFile(std::string relative_path, std::string file_name, std::string content) {
        auto absolute_dir_path = std::filesystem::path(m_root_data_path) / relative_path;
        if (std::filesystem::create_directories(absolute_dir_path) || std::filesystem::exists(absolute_dir_path)) {
            auto absolute_path = absolute_dir_path / file_name;
            std::ofstream file(absolute_path, std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                return false;
            }
            file << content;
            return file.good();
        }
        return false;
    }

    Result<std::string> FileManager::ReadTextFromFile(const std::string& relative_path) {
        std::string error_msg;
        std::filesystem::path file_path;
        std::error_code error_code;

        if (!ValidatePath(relative_path, file_path, error_msg, error_code)) {
            return FileNotFound<std::string>();
        }

        std::ifstream file(file_path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            return Fail<std::string>("Failed to open file: " + file_path.string(), 0);
        }

        const auto file_size = file.tellg();
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();
        return Result<std::string>{
            .value = std::string{buffer.data(), static_cast<size_t>(file_size)},
            .error = LoadError{}
        };
    }

    Result<std::string> FileManager::ReadTextFromResourceFile(const std::string& relative_path) {
        std::string error_msg;
        std::filesystem::path file_path;
        std::error_code error_code;
        if (!ValidatePath(m_resource_dir_name + "/" + relative_path, file_path, error_msg, error_code)) {
            return FileNotFound<std::string>();
        }

        std::ifstream file(file_path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            return Fail<std::string>("Failed to open file: " + file_path.string(), 0);
        }

        const auto file_size = file.tellg();
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();
        return Result<std::string>{
            .value = std::string{buffer.data(), static_cast<size_t>(file_size)},
            .error = LoadError{}
        };
    }

    Result<FileBinary> FileManager::ReadBinaryFromResourceFile(const std::string& relative_path) {
        std::string error_msg;
        std::filesystem::path file_path;
        std::error_code error_code;
        if (!ValidatePath(m_resource_dir_name + "/" + relative_path, file_path, error_msg, error_code)) {
            return FileNotFound<FileBinary>();
        }
        std::ifstream ifs(file_path, std::ios::binary | std::ios::ate);
        if (!ifs) {
            return Fail<FileBinary>("Failed to open file: " + file_path.string(), 0);
        }

        const std::streamsize file_size = ifs.tellg();
        if (file_size < 0) {
            return Fail<FileBinary>("Failed to read file: " + file_path.string(), 0);
        }

        constexpr auto max_size = std::numeric_limits<size_t>::max();
        if (static_cast<std::uintmax_t>(file_size) > max_size) {
            return Fail<FileBinary>("File size exceeds maximum size", 0);
        }

        ifs.seekg(0, std::ios::beg);

        auto buffer = std::vector<uint8_t>(file_size);
        if (!ifs.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
            return Fail<FileBinary>("Failed to read file: " + file_path.string(), 0);
        }

        uint64_t mtime = 0;
        if (!error_code) {
            auto ftime = std::filesystem::last_write_time(file_path, error_code);
            if (!error_code) {
                const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now()
                        );
                mtime = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(
                        sctp.time_since_epoch()
                        ).count());
            }
        }
        FileBinary out;
        out.data = std::move(buffer);
        out.size = static_cast<size_t>(file_size);
        out.name = file_path.filename().string();
        out.path = file_path.string();
        out.mtime = mtime;


        return Result{
            .value = std::move(out)
        };
    }

    ResultCollection<FilePath> FileManager::FindResourceFilesOfTypes(
            const std::string& directory,
            const std::vector<std::string>& file_types) {
        if (file_types.empty()) {
            return InvalidFileExtension<FilePath>("File extensions list was empty!");
        }

        const auto base_path = std::filesystem::path(m_root_data_path);
        const auto directory_path = base_path / directory;


        std::vector<FilePath> result;
        for (const auto& file_type: file_types) {
            if (file_type.empty()) {
                return InvalidFileExtension<FilePath>("File type is empty!");
            }
            std::string file_type_normalized(file_type);
            std::ranges::transform(file_type_normalized, file_type_normalized.begin(), ::tolower);

            for (const auto& entry: std::filesystem::directory_iterator{directory_path}) {
                if (entry.is_regular_file() && entry.path().extension() == file_type_normalized) {
                    auto relative = std::filesystem::relative(entry.path(), directory_path);
                    result.emplace_back(file_type, relative.string());
                }
            }
        }
        return ResultCollection{
            .type = ResultType::Ok,
            .value = std::move(result),
            .error = "",
        };
    }

    bool FileManager::ValidatePath(const std::string& relative_path, std::filesystem::path& path, std::string& error,
                                   std::error_code& error_code) {
        if (relative_path.empty()) {
            throw std::invalid_argument("relative file path is null");
        }
        const auto base = std::filesystem::path(m_root_data_path);
        std::filesystem::path resource_path = base;

        auto relative_file_path = std::filesystem::path(relative_path);
        for (const auto& file: relative_file_path) {
            if (file.string().starts_with(".") ||
                file.string().starts_with("..") ||
                file.string().find('/') == 0 ||
                file.string().find('\\') == 0 ||
                file.string().find(':') == 0) {
                throw std::invalid_argument("Invalid file path: " + relative_file_path.string());
            }

            resource_path /= file;
        }

        resource_path = weakly_canonical(resource_path);
        auto rel = resource_path.lexically_relative(base);

        if (rel.empty()) {
            throw std::invalid_argument("Invalid file path: " + relative_file_path.string());
        }
        for (const auto& file: rel) {
            if (file.string().starts_with("..")) {
                throw std::invalid_argument("Invalid file path: " + relative_file_path.string());
            }
        }

        path = resource_path;
        std::error_code ec;

        if (!exists(path, ec) || !is_regular_file(path, ec)) {
            error = "Failed to open file: " + path.string();
            error_code = ec;
            return false;
        }
        return true;
    }

    template<typename T>
    Result<T> FileManager::Success(T value) {
        Result<T> result;
        result.value = std::move(value);
        result.type = ResultType::Ok;
        return result;
    }

    template<typename T>
    Result<T> FileManager::Fail(const std::string& msg, const int code) {
        Result<T> result;
        result.error = LoadError{
            .message = msg,
            .code = code
        };
        result.type = ResultType::FileError;
        return result;
    }

    template<typename T>
    Result<T> FileManager::FileNotFound() {
        Result<T> result;
        result.type = ResultType::FileNotFound;
        return result;
    }

    template<typename T>
    ResultCollection<T> FileManager::InvalidFileExtension(const std::string& msg) {
        ResultCollection<T> result;
        result.type = ResultType::InvalidFileExtension;
        result.error = LoadError{.message = msg, .code = 42};
        return result;
    }
} // namespace
