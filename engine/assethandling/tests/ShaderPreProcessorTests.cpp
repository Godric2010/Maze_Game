#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../src/Shader/ShaderPreProcessor.hpp"


TEST_CASE("ShaderPreProcessorTests - Include gets replaced with included shader") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "Content of helper A"},
        {"helperB", "Content of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include <helperA>\n#include <helperB>\nTest string end";
    auto processed = pre_processor.PreProcessShader(test_string);

    const std::string expected = "Test string start\nContent of helper A\n\nContent of helper B\n\nTest string end\n";

    REQUIRE(processed == expected);
}

TEST_CASE("ShaderPreProcessorTests - Recursive include gets resolved correctly") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "Content of helper A"},
        {"helperB", "#include <helperA>\nContent of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include <helperB>\nTest string end";
    auto processed = pre_processor.PreProcessShader(test_string);

    const std::string expected = "Test string start\nContent of helper A\n\nContent of helper B\n\nTest string end\n";

    REQUIRE(processed == expected);
}

TEST_CASE("ShaderPreProcessorTest - Each include only happens once") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "Content of helper A"},
        {"helperB", "#include <helperA>\nContent of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include <helperA>\n#include <helperB>\nTest string end";
    auto processed = pre_processor.PreProcessShader(test_string);

    const std::string expected = "Test string start\nContent of helper A\n\nContent of helper B\n\nTest string end\n";

    REQUIRE(processed == expected);
}

TEST_CASE("ShaderPreProcessorTest - Empty include throws exception") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "Content of helper A"},
        {"helperB", "#include <helperA>\nContent of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include \n#include <helperB>\nTest string end";
    REQUIRE_THROWS(pre_processor.PreProcessShader(test_string));
}

TEST_CASE("ShaderPreProcessorTest - Invalid include throws exception") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "Content of helper A"},
        {"helperB", "#include <helperA>\nContent of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include <helperC>\nTest string end";
    REQUIRE_THROWS(pre_processor.PreProcessShader(test_string));
}

TEST_CASE("ShaderPreProcessorTest - Cyclic include throws exception") {
    const std::unordered_map<std::string, std::string> helper_map = {
        {"helperA", "#include <helperB>\nContent of helper A"},
        {"helperB", "#include <helperA>\nContent of helper B"},
    };

    auto pre_processor = Engine::AssetHandling::Shader::ShaderPreProcessor(helper_map);
    const std::string test_string = "Test string start\n#include <helperB>\nTest string end";
    REQUIRE_THROWS(pre_processor.PreProcessShader(test_string));
}
