REM Build and run utility script.

:: Create the build directory if it does not exist.
if not exist Build/ (
    mkdir Build
)

:: Change directory to the build directory and build using CMake.
cd Build
cmake ..

:: Setup the configuration if the user has provided it and run accordingly.
if "%1" == "" (
    cmake --build .

    :: Run the built Desktop executable.
    call "Source/Desktop/Debug/Desktop.exe"
) else (
    cmake --build . --config "%1"

    :: Run the built Desktop executable.
    call "Source/Desktop/%1/Desktop.exe"
)
