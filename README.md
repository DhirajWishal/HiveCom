# HiveCom

Communications protocol for drones in a flying ad hoc network.

## How to run?

### Prerequisites

Before we can proceed, make sure the following requirements are met.

- CMake v3.22.3 or newer
- OpenSSL v3.0.0 or newer
- C++ compiler

### Initial setup

Clone the repository and initialize the submodules.

```sh
git clone https://github.com/DhirajWishal/HiveCom
cd HiveCom
git submodule update --init
```

### Build the framework

Run the following commands to build the framework.

```sh
mkdir Build
cd Build
cmake .. && cmake --build . --config Release
```

### Running the built framework

The built framework is placed inside the `Build` directory. The desktop application can be found inside the `Build/Source/Desktop/Release/` folder. You can either manually run using the following command.

```sh
./Source/Desktop/Release/Desktop.exe
```

Or optionally, an helper script is provided under the `Scripts` directory.
