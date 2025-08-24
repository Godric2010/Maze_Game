cmake_minimum_required(VERSION 3.31)

add_library(gameplay STATIC TestSystem.hpp TestSystem.cpp)

target_link_libraries(gameplay PUBLIC Engine)