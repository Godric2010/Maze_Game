#include "ShaderPreProcessor.hpp"

#include <sstream>
#include <utility>

namespace Engine::AssetHandling::Shader {
    ShaderPreProcessor::ShaderPreProcessor(std::unordered_map<std::string, std::string> helper_content_map) {
        m_shader_helper_content_map = std::move(helper_content_map);
    }

    std::string ShaderPreProcessor::PreProcessShader(const std::string& shader_content) {
        m_include_stack.clear();
        m_include_set.clear();

        auto processed_shader = ResolveIncludesRecursive(shader_content);
        return processed_shader;
    }

    std::string ShaderPreProcessor::ResolveIncludesRecursive(const std::string& source) {
        const std::vector<std::string> lines = SplitLines(source);
        std::stringstream result;
        int line_number = 0;


        for (const auto& line: lines) {
            line_number++;
            const std::string trimmed = Trim(line);

            if (!IsIncludeLine(trimmed)) {
                result << line << "\n";
                continue;
            }

            std::string include_name = ExtractIncludeName(trimmed);

            if (include_name.empty()) {
                throw std::runtime_error("No include name given at line number: " + std::to_string(line_number));
            }

            if (StackContainsIncludeAlready(include_name)) {
                throw std::runtime_error(BuildCycleMessage(include_name));
            }

            auto it = m_shader_helper_content_map.find(include_name);
            if (it == m_shader_helper_content_map.end()) {
                throw std::runtime_error("Shader include not found: " + include_name);
            }

            if (m_include_set.contains(include_name)) {
                continue;
            }
            m_include_set.insert(include_name);
            m_include_stack.push_back(include_name);

            const std::string resolved_include = ResolveIncludesRecursive(it->second);
            m_include_stack.pop_back();
            result << resolved_include << "\n";
        }

        return result.str();
    }

    std::vector<std::string> ShaderPreProcessor::SplitLines(const std::string& source) {
        std::vector<std::string> lines;
        std::stringstream stream(source);
        std::string line;

        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    bool ShaderPreProcessor::IsIncludeLine(const std::string& line) {
        return line.find("#include") != std::string::npos;
    }

    std::string ShaderPreProcessor::ExtractIncludeName(const std::string& line) {
        const size_t start = line.find('<');
        const size_t end = line.find('>', start + 1);

        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }

        return line.substr(start + 1, end - start - 1);
    }

    std::string ShaderPreProcessor::Trim(const std::string& std) {
        const auto whitespace = "\t\n\r";
        const size_t start = std.find_first_not_of(whitespace);
        const size_t end = std.find_last_not_of(whitespace);
        if (start == std::string::npos) {
            return "";
        }
        return std.substr(start, end - start + 1);
    }

    bool ShaderPreProcessor::StackContainsIncludeAlready(const std::string& value) const {
        for (const auto& stack_val: m_include_stack) {
            if (stack_val == value) {
                return true;
            }
        }
        return false;
    }

    std::string ShaderPreProcessor::BuildCycleMessage(const std::string& repeated_include) const {
        std::stringstream stream;
        stream << "Cyclic shader include detected: ";
        for (const auto& stack_val: m_include_stack) {
            stream << stack_val << " -> ";
        }
        stream << repeated_include;
        return stream.str();
    }
} // namespace
