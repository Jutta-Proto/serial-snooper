cmake_minimum_required(VERSION 3.16)

add_library(jutta_snooper SHARED JuttaSnooper.hpp
                                 JuttaSnooper.cpp)
target_link_libraries(jutta_snooper PRIVATE jutta_proto logger)

install(TARGETS jutta_snooper)