cmake_minimum_required(VERSION 3.31)

add_library(Text STATIC
        TextDummy.cpp
        TextDummy.hpp)

if (COMMAND enable_coverage)
    message("Text- Enable coverage")
    enable_coverage(Text)
endif ()
