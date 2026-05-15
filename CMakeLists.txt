cmake_minimum_required(VERSION 3.10)
project(SerialLibrary VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
endif()

# Library source files
set(SERIAL_SOURCES
    serial.cpp
)

set(SERIAL_HEADERS
    serial.h
)

# byte-stuffing dependency (bytestream target)
if(NOT TARGET bytestream)
    add_subdirectory(external/byte-stuffing)
endif()

# Create the serial library
add_library(serial STATIC ${SERIAL_SOURCES} ${SERIAL_HEADERS})

# Set library properties
set_target_properties(serial PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${SERIAL_HEADERS}
)

# Platform-specific settings
if(PLATFORM_WINDOWS)
    # Windows-specific settings
    target_compile_definitions(serial PRIVATE _WIN32)
    # No additional libraries needed for Windows serial communication
elseif(PLATFORM_LINUX)
    # Linux-specific settings
    target_compile_definitions(serial PRIVATE __linux__)
    target_link_libraries(serial PRIVATE pthread)
elseif(PLATFORM_MACOS)
    # macOS-specific settings
    target_compile_definitions(serial PRIVATE __APPLE__)
    target_link_libraries(serial PRIVATE pthread)
endif()

target_link_libraries(serial PUBLIC bytestream)

# Include directories
target_include_directories(serial
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler-specific settings
if(MSVC)
    target_compile_options(serial PRIVATE /W4)
else()
    target_compile_options(serial PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(SERIAL_BUILD_EXAMPLES "Build serial example executable" OFF)
if(SERIAL_BUILD_EXAMPLES)
    add_executable(serial_example serial_example.cpp)
    target_link_libraries(serial_example serial)
endif()


# Testing (optional)
option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
    enable_testing()
    # Add tests here if needed
endif()

# Print configuration summary
message(STATUS "Serial Library Configuration:")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
if(PLATFORM_WINDOWS)
    message(STATUS "  Windows Serial API")
elseif(PLATFORM_LINUX)
    message(STATUS "  Linux Serial API (termios)")
elseif(PLATFORM_MACOS)
    message(STATUS "  macOS Serial API (termios)")
endif() 