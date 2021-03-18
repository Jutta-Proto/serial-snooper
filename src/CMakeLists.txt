cmake_minimum_required(VERSION 3.16)

set(MAIN_EXECUTABLE ${PROJECT_NAME})

add_executable(${MAIN_EXECUTABLE} main.cpp)

target_link_libraries(${MAIN_EXECUTABLE} PRIVATE logger jutta_snooper serial)
set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS JUTTA_SNOOPER_VERSION="${PROJECT_VERSION}" JUTTA_SNOOPER_VERSION_NAME="${VERSION_NAME}")

add_subdirectory(jutta_snooper)