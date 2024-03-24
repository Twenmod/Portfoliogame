cmake_minimum_required(VERSION 3.10)
project(Spelunker)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define output directory for Windows and Linux
if(WIN32)
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/windows32")
elseif(UNIX)
    set(OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/linux")
endif()

# Ensure that the output directory exists
file(MAKE_DIRECTORY "${OUTPUT_DIRECTORY}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set sfml path
set(SFML_DIR "${PROJECT_SOURCE_DIR}/lib/SFML/lib/cmake/SFML")

# Find SFML library
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics network window audio system)


# Add source files
file(GLOB SOURCES "src/*.cpp")


# Create executable
add_executable(Spelunker ${SOURCES})

# Link SFML libraries
target_link_libraries(Spelunker PRIVATE sfml-graphics sfml-network sfml-window sfml-audio sfml-system)

# Copy SFML DLL files after build on Windows
if(WIN32)
    add_custom_command(TARGET Spelunker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML/bin
            $<TARGET_FILE_DIR:Spelunker>)
endif()

# Remove old assets and Copy new assets
add_custom_command(TARGET Spelunker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:Spelunker>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Spelunker>/assets)
