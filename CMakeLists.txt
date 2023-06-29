cmake_minimum_required(VERSION 3.21)
project(device-simple-PTP CXX)

set(SOURCES
    ./src/main.cpp
)

# Dependencies
find_package(edgex-csdk REQUIRED)

# Library definition
add_executable(device-simple-PTP ${SOURCES})
target_link_libraries(device-simple-PTP PUBLIC EdgeX::csdk)

# Installation
include(GNUInstallDirs)
install(TARGETS device-simple-PTP)
