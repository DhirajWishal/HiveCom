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

target_link_libraries(Core oqs)

target_include_directories(
    Core 
    
    PUBLIC ${LIBOQS_INCLUDE_DIR}
)
