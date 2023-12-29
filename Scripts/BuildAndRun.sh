# Build and run utility script.

# Create the build directory if it doesn't exist.
if [ ! -d "Build" ]; then
    mkdir Build
fi

# Change directory to the build directory and build using CMake.
cd Build
cmake ..

# Setup the configuration if the user has provided it and run accordingly.
if [ $1 -eq "" ] 
then 
    cmake --build .

else
    cmake --build . --config "%1"

fi 

# Run the built Desktop executable.
./Source/Desktop/Desktop
