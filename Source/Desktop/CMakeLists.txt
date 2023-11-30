# Set the project name.
project(
	Desktop
)

# Set the project sources.
set(
	SOURCES

	"${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
)

# Setup the library information.
add_executable(
	Desktop 

	${SOURCES}
)
