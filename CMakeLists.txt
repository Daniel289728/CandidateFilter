cmake_minimum_required(VERSION 3.14)
project(CandidateFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add external dependencies
include_directories(${PROJECT_SOURCE_DIR}/external)

# Source files
set(SOURCES
    src/main.cpp
    src/downloader.cpp
    src/parser.cpp
    src/candidate.cpp
)

# Create executable
add_executable(CandidateFilter ${SOURCES})

# Add required libraries
find_package(CURL REQUIRED)
target_link_libraries(CandidateFilter CURL::libcurl)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/danie/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
