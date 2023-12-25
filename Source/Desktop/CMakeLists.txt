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
	"Simulator/Node.cpp"
	"Simulator/Node.hpp"
	"Simulator/NetworkGrid.cpp"
	"Simulator/NetworkGrid.hpp"
	"Simulator/RandomizedRouterNode.cpp"
	"Simulator/RandomizedRouterNode.hpp"
	"Simulator/Message.cpp"
	"Simulator/Message.hpp"
)

# Setup the library information.
add_executable(
	Desktop 

	${SOURCES}
)
