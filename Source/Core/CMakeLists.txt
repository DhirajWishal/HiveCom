# Set the project name.
project(
	Core
)

# Set the project sources.
set(
	SOURCES

	"AES256.cpp"
	"AES256.hpp"
)

# Setup the library information.
add_library(
	Core 
	STATIC

	${SOURCES}
)
