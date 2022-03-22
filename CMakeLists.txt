cmake_minimum_required(VERSION 3.16)

set(PROJECT_DESCRIPTION " C++ serial snooper for JURA coffee makers.")

project("Jutta_Snooper" 
            VERSION 0.0.0
            DESCRIPTION "${PROJECT_DESCRIPTION}"
            HOMEPAGE_URL "https://github.com/Jutta-Proto/serial-snooper")
set(VERSION_NAME "dev")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(jutta_snooper_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

message(STATUS "C++ Jutta UI Options")
message(STATUS "=======================================================")
jutta_snooper_option(JUTTA_SNOOPER_STATIC_ANALYZE "Set to ON to enable the GCC 10 static analysis. If enabled, JUTTA_SNOOPER_ENABLE_LINTING has to be disabled." OFF)
jutta_snooper_option(JUTTA_SNOOPER_ENABLE_LINTING "Set to ON to enable clang linting. If enabled, JUTTA_SNOOPER_STATIC_ANALYZE has to be disabled." ON)
message(STATUS "=======================================================")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(sanitizer)
include(gcc_analyze)
include(clear_variable)
include(FetchContent)

if (${JUTTA_SNOOPER_ENABLE_LINTING})
    message(STATUS "Enabling linting")
    include(clang-tidy)
else()
    message(STATUS "Linting is disabled")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

set(CONAN_CONFIGS"Release;Debug;RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE IN_LIST CONAN_CONFIGS)
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

conan_cmake_run(REQUIRES spdlog/1.8.5
                GENERATORS cmake_find_package
                BUILD missing)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(spdlog REQUIRED)

# Disable linting for jutta_proto
clear_variable(DESTINATION CMAKE_CXX_CLANG_TIDY BACKUP CMAKE_CXX_CLANG_TIDY_BKP)

FetchContent_Declare(jutta_proto
                     GIT_REPOSITORY         https://github.com/Jutta-Proto/protocol-cpp.git
                     GIT_TAG main
                     USES_TERMINAL_DOWNLOAD TRUE)   # <---- This is needed only for Ninja to show download progress
set(JUTTA_PROTO_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(JUTTA_PROTO_STATIC_ANALYZE OFF CACHE INTERNAL "" FORCE)
set(JUTTA_PROTO_ENABLE_LINTING OFF CACHE INTERNAL "" FORCE)
FetchContent_MakeAvailable(jutta_proto)

restore_variable(DESTINATION CMAKE_CXX_CLANG_TIDY BACKUP CMAKE_CXX_CLANG_TIDY_BKP)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
