
# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.22.3)

# Set the main project information.
project(
	HiveCom

	VERSION 0.0.1
	DESCRIPTION "Secure communications protocol for drones based on a flying ad hoc network"
)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 20)

# Add the subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Desktop)
