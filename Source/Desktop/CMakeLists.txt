# Set the project name.
project(
	Desktop
)

# Set the project sources.
set(
	SOURCES

	"Main.cpp"

	"Simulator/Reactor.cpp"
	"Simulator/Reactor.hpp"
	"Simulator/ThreadPool.hpp"
)

# Setup the library information.
add_executable(
	Desktop 

	${SOURCES}
)
