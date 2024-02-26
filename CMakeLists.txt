cmake_minimum_required(VERSION 3.10)
project(Spelunker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define output directory for Windows and Linux
if(WIN32)
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/windows")
elseif(UNIX)
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/linux")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")

# Fix for Vcpkg on Windows
if(WIN32)
    include($ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set sfml path
set(SFML_DIR "${PROJECT_SOURCE_DIR}/lib/SFML/lib/cmake/SFML")

# Find SFML library
find_package(SFML 2.6 REQUIRED COMPONENTS graphics network window audio system)

# And jsonCPP
find_package(jsoncpp CONFIG REQUIRED)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(Spelunker ${SOURCES})

# Link SFML and json libraries
target_link_libraries(Spelunker PRIVATE sfml-graphics sfml-network sfml-window sfml-audio sfml-system JsonCpp::JsonCpp)

# Copy SFML DLL files after build on Windows
if(WIN32)
    add_custom_command(TARGET Spelunker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/lib/SFML/bin
            $<TARGET_FILE_DIR:Spelunker>)
endif()

# Copy assets
add_custom_command(TARGET Spelunker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        $<TARGET_FILE_DIR:Spelunker>)

# Copy settings
add_custom_command(TARGET Spelunker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/settings.json
        $<TARGET_FILE_DIR:Spelunker>)