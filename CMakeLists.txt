cmake_minimum_required(VERSION 3.21)
project(device-simple-PTP CXX)

add_compile_definitions(USE_EATON_SDK_ENHANCEMENT)


file(GLOB_RECURSE src_files src/*.cpp)
set(SOURCES
    ./src/main.cpp
)

add_executable(device-simple-PTP ${SOURCES})

# Dependencies
#find_package(edgex-csdk REQUIRED)
#target_include_directories (device-simple-PTP  PRIVATE inc/)
target_include_directories (device-simple-PTP PRIVATE inc/)
target_link_libraries (device-simple-PTP PRIVATE csdk)
# target_link_libraries (device-simple-PTP PRIVATE csdk curl microhttpd yaml cbor hiredis paho-mqtt3as uuid)

# Library definition

#target_link_libraries(device-simple-PTP PUBLIC EdgeX::csdk)

# Installation
include(GNUInstallDirs)
install(TARGETS device-simple-PTP)
install(DIRECTORY res DESTINATION res)
