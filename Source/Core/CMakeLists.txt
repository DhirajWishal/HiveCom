# Set the project name.
project(
	Core
)

# Set the project sources.
set(
	SOURCES

	"${CMAKE_CURRENT_SOURCE_DIR}/AES256.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/AES256.hpp"
)

# Setup the library information.
add_library(
	Core 
	STATIC

	${SOURCES}
)
