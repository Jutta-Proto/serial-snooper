cmake_minimum_required(VERSION 3.16)

set(PROJECT_DESCRIPTION " C++ serial snooper for JURA coffee makers.")

project("Jutta_Snooper" 
            VERSION 0.0.0
            DESCRIPTION "${PROJECT_DESCRIPTION}"
            HOMEPAGE_URL "https://github.com/Jutta-Proto/serial-snooper")
set(VERSION_NAME "dev")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_UNITY_BUILD "Enable unity build" OFF)
option(ENABLE_LINTING "Enables linting while building" ON)
option(BUILD_TESTING "Build tests" OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(sanitizer)
include(gcc_analyze)
include(FetchContent)

if (${ENABLE_LINTING})
    message(STATUS "Enabling linting")
    include(clang-tidy)
else()
    message(STATUS "Linting is disabled")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT isMultiConfig)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${ALLOWED_BUILD_TYPES}")
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    elseif(NOT CMAKE_BUILD_TYPE IN_LIST ALLOWED_BUILD_TYPES)
        message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
    endif()
else ()
    unset(CMAKE_BUILD_TYPE)
    foreach(TYPE ${ALLOWED_BUILD_TYPES})
    if (NOT ${TYPE} IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES ${TYPE})
    endif()  
    endforeach()  
endif()

set(CONAN_CONFIGS"Release;Debug;RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE IN_LIST CONAN_CONFIGS)
    set(CONAN_BUILD_TYPE "Debug")
else()
    set(CONAN_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

FetchContent_Declare(jutta_proto
                     GIT_REPOSITORY         https://github.com/Jutta-Proto/protocol-cpp.git
                     GIT_TAG main
                     USES_TERMINAL_DOWNLOAD TRUE)   # <---- This is needed only for Ninja to show download progress
FetchContent_MakeAvailable(jutta_proto)

add_subdirectory(src)
