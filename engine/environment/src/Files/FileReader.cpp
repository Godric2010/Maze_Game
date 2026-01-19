#include "FileReader.hpp"

#include <fstream>

#include "ExecutablePath.hpp"

namespace Engine::Environment::Files {
    FileReader::FileReader() {
        m_root_data_path = GetExecutableDirectory().string();
    }

    Result<std::string> FileReader::ReadTextFromFile(const std::string& relative_path) {
        std::string error_msg;
        std::filesystem::path file_path;
        std::error_code error_code;
        if (!ValidatePath(relative_path, file_path, error_msg, error_code)) {
            return Fail<std::string>(error_msg, -1);
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

    Result<FileBinary> FileReader::ReadBinaryFromFile(const std::string& relative_path) {
        std::string error_msg;
        std::filesystem::path file_path;
        std::error_code error_code;
        if (!ValidatePath(relative_path, file_path, error_msg, error_code)) {
            return Fail<FileBinary>(error_msg, -1);
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

    bool FileReader::ValidatePath(const std::string& relative_path, std::filesystem::path& path, std::string& error,
                                  std::error_code& error_code) {
        if (relative_path.empty()) {
            throw std::invalid_argument("relative file path is null");
        }
        const auto base = std::filesystem::path(m_root_data_path) / m_resource_dir_name;
        std::filesystem::path resource_path = base;

        auto relative_file_path = std::filesystem::path(relative_path);
         for (const auto& file: relative_file_path) {
            if (file.string().starts_with(".") ||
                file.string().starts_with("..") ||
                file.string().find('/') == 0 ||
                file.string().find('\\') == 0 ||
                file.string().find(':') == 0){
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
    Result<T> FileReader::Fail(const std::string& msg, const int code) {
        Result<T> result;
        result.error = LoadError{
            .message = msg,
            .code = code
        };
        return result;
    }
} // namespace
