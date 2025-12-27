#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

namespace Engine::Environment {
    struct FileData {
        std::vector<uint8_t> data;
        size_t size = 0;
        std::string name;
        std::filesystem::path path;
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

    class FileReader {
    public:
        static Result<std::string> ReadTextContentFromFile(const std::string &filepath) {
            const std::filesystem::path base = std::filesystem::current_path() / "resources";
            std::filesystem::path resource_path = base / filepath;

            resource_path = weakly_canonical(resource_path);
            if (!resource_path.string().starts_with(base.string())) {
                return Fail<std::string>("Path traversal attempt detected", -1);
            }
            std::error_code ec;

            if (!exists(resource_path, ec) || !is_regular_file(resource_path, ec)) {
                return Fail<std::string>("Failed to open file: " + resource_path.string(), ec.value());
            }

            std::ifstream file(resource_path, std::ios::ate | std::ios::binary);
            if (!file.is_open()) {
                return Fail<std::string>("Failed to open file: " + resource_path.string(), 0);
            }
            const size_t file_size = (size_t) file.tellg();
            std::vector<char> buffer(file_size);
            file.seekg(0);
            file.read(buffer.data(), file_size);
            file.close();
            return Result<std::string>{
                .value = std::string{buffer.data(), file_size},
                .error = LoadError{}
            };
        }

        static Result<FileData> LoadBinaryFromFile(const std::string &filepath) {
            const std::filesystem::path base = std::filesystem::current_path() / "resources";
            std::filesystem::path resource_path = base / filepath;

            resource_path = weakly_canonical(resource_path);
            if (!resource_path.string().starts_with(base.string())) {
                return Fail<FileData>("Path traversal attempt detected", -1);
            }
            std::error_code ec;

            if (!exists(resource_path, ec) || !is_regular_file(resource_path, ec)) {
                return Fail<FileData>("Failed to open file: " + resource_path.string(), ec.value());
            }

            std::ifstream ifs(resource_path, std::ios::binary | std::ios::ate);
            if (!ifs) {
                return Fail<FileData>("Failed to open file: " + resource_path.string(), 0);
            }

            const std::streamsize file_size = ifs.tellg();
            if (file_size < 0) {
                return Fail<FileData>("Failed to read file: " + resource_path.string(), 0);
            }

            constexpr auto max_size = std::numeric_limits<size_t>::max();
            if (static_cast<std::uintmax_t>(file_size) > max_size) {
                return Fail<FileData>("File size exceeds maximum size", 0);
            }

            ifs.seekg(0, std::ios::beg);

            auto buffer = std::vector<uint8_t>(file_size);
            if (!ifs.read(reinterpret_cast<char *>(buffer.data()), file_size)) {
                return Fail<FileData>("Failed to read file: " + resource_path.string(), 0);
            }

            uint64_t mtime = 0;
            if (!ec) {
                auto ftime = std::filesystem::last_write_time(resource_path, ec);
                if (!ec) {
                    const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                        ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now());
                    mtime = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(
                        sctp.time_since_epoch()).count());
                }
            }
            FileData out;
            out.data = std::move(buffer);
            out.size = static_cast<size_t>(file_size);
            out.name = resource_path.filename().string();
            out.path = resource_path;
            out.mtime = mtime;


            return Result{
                .value = std::move(out)
            };
        }

    private:
        template<typename T>
        static Result<T> Fail(const std::string &msg, const int code) {
            Result<T> result;
            result.error = LoadError{
                .message = msg,
                .code = code
            };
            return result;
        }
    };
} // namespace
