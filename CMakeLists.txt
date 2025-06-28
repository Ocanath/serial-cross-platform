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
    # Link against required libraries for Linux serial communication
    target_link_libraries(serial pthread)
elseif(PLATFORM_MACOS)
    # macOS-specific settings
    target_compile_definitions(serial PRIVATE __APPLE__)
    # macOS uses the same termios interface as Linux
    target_link_libraries(serial pthread)
endif()

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

# Create example executable
add_executable(serial_example serial_example.cpp)
target_link_libraries(serial_example serial)

# Installation configuration
include(GNUInstallDirs)

# Install the library
install(TARGETS serial
    EXPORT SerialLibraryTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/serial
)

# Install headers
install(FILES ${SERIAL_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/serial
)

# Export targets
install(EXPORT SerialLibraryTargets
    FILE SerialLibraryTargets.cmake
    NAMESPACE SerialLibrary::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SerialLibrary
)

# Create and install config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    SerialLibraryConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/SerialLibraryConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SerialLibraryConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SerialLibrary
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SerialLibraryConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SerialLibraryConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SerialLibrary
)

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