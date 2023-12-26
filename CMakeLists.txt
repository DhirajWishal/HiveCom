
# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.22.3)

# Set the main project information.
project(
	HiveCom

	VERSION 0.0.1
	DESCRIPTION "Secure communications protocol for drones based on a flying ad hoc network"
)

find_package(OpenSSL REQUIRED)

# Set global flags.
set(CMAKE_CXX_STANDARD 20)

# Set flags used by third party and our own projects.
set(LIBOQS_INCLUDE_DIR "${CMAKE_BINARY_DIR}/ThirdParty/liboqs/include")
message(STATUS "liboqs include directory set to ${LIBOQS_INCLUDE_DIR}")

set(OQS_BUILD_ONLY_LIB ON) # build only the liboqs library and not anything else.

# Uncomment this when building for embedded systems.
# set(OQS_EMBEDDED_BUILD  ON)

# Add the third party subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/liboqs)

# Add the subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Desktop)

# Setup the compile_flags.txt file to assist clangd.
file(
    WRITE ${CMAKE_CURRENT_SOURCE_DIR}/compile_flags.txt

    "-std=c++${CMAKE_CXX_STANDARD}\n"
    "-I${LIBOQS_INCLUDE_DIR}\n"
    "-I${OPENSSL_INCLUDE_DIR}\n"
)
