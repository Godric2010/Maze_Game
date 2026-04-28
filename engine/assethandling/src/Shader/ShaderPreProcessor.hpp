#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Engine::AssetHandling::Shader {
    class ShaderPreProcessor {
    public:
        explicit ShaderPreProcessor(std::unordered_map<std::string, std::string> helper_content_map);

        ~ShaderPreProcessor() = default;

        std::string PreProcessShader(const std::string& shader_content);

    private:
        std::unordered_map<std::string, std::string> m_shader_helper_content_map;
        std::vector<std::string> m_include_stack;
        std::unordered_set<std::string> m_include_set;

        std::string ResolveIncludesRecursive(const std::string& source);

        static std::vector<std::string> SplitLines(const std::string& source);

        static bool IsIncludeLine(const std::string& line);

        static std::string ExtractIncludeName(const std::string& line);

        static std::string Trim(const std::string& std);

        bool StackContainsIncludeAlready(const std::string& value) const;

        std::string BuildCycleMessage(const std::string& repeated_include) const;
    };
} // namespace
