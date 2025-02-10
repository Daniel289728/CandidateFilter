cmake_minimum_required(VERSION 3.14)
project(CandidateFilter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the VCPKG toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/danie/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Add vcpkg's installed package directory to CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "C:/Users/danie/vcpkg/installed/x64-windows")

# Include directories for external dependencies
include_directories(${PROJECT_SOURCE_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/src/backend)
include_directories(C:/Users/danie/vcpkg/installed/x64-windows/include/skia)


# Source files
set(SOURCES
    src/ui/main.cpp
    src/backend/downloader.cpp
    src/backend/parser.cpp
    src/backend/candidate.cpp
    src/backend/candidateFilter.cpp
    src/backend/candidateScorer.cpp
    src/backend/candidateFilter.cpp
    src/backend/candidateScorer.cpp
    src/ui/SkiaUI.cpp
)

# Create executable
add_executable(CandidateFilter ${SOURCES})

# Add required libraries
find_package(CURL REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(ZLIB REQUIRED)  # Add this line for ZLIB
find_package(unofficial-skia CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Link libraries
target_link_libraries(CandidateFilter PRIVATE 
    CURL::libcurl 
    pugixml::shared pugixml::pugixml
    nlohmann_json::nlohmann_json
    ZLIB::ZLIB  # Add this line to link ZLIB
    unofficial::skia::skia
    glfw
)
