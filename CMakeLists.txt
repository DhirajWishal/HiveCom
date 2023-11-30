
# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.27.6)

# Set the main project information.
project(
	HiveCom

	VERSION 0.0.1
	DESCRIPTION "Secure communications protocol for drones based on a flying ad hoc network"
)

# Add the subdirectories.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Source/Desktop)
