cmake_minimum_required(VERSION 3.29)
project(puffers LANGUAGES CXX C)
set(PROJECT_NAME puffers)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_CXX_EXTENSIONS OFF)  # optional, keep compiler extensions off
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(COPYRIGHT "Copyright 2024, GrokkingLab")
set(VENDOR "GrokkingLab")
set(DESCRIPTION "A simple game using C++ and web technologies")

# Define the source and destination directories
set(WEB_SOURCE_DIR "${CMAKE_SOURCE_DIR}/web")
set(BIN_DEST_DIR "${CMAKE_BINARY_DIR}/bin")

# Copy the web directory to the bin directory
file(COPY ${WEB_SOURCE_DIR} DESTINATION ${BIN_DEST_DIR})

find_package(Threads REQUIRED)
include(FetchContent)
include_directories(include)
include_directories(third_party)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(WEBVIEW_BUILD_SHARED_LIBRARY OFF)
set(WEBVIEW_BUILD_STATIC_LIBRARY ON)
set(WEBVIEW_BUILD_SHARED OFF)
set(BUILD_STATIC ON)
FetchContent_Declare(
        webview
        GIT_REPOSITORY https://github.com/webview/webview
        GIT_TAG 0.12.0)
FetchContent_MakeAvailable(webview)

# Spdlog
set(SPDLOG_BUILD_EXAMPLES OFF)
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
set(SPDLOG_FMT_EXTERNAL_HO OFF)

# Graaf
set(SKIP_TESTS ON)
set(SKIP_EXAMPLES ON)
set(SKIP_BENCHMARKS ON)

add_subdirectory(third_party/graaf)
add_subdirectory(third_party/spdlog)

find_library(COREGRAPHICS_LIBRARY CoreGraphics)
add_executable(${PROJECT_NAME} main.cpp
               include/Log.hpp
               include/FileUtils.hpp
               include/ScreenUtils.hpp)

# Set C++23 for the main project target
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} PRIVATE
                      webview::core
                      spdlog::spdlog
                      Graaf::Graaf
                      ${COREGRAPHICS_LIBRARY}
                      )
