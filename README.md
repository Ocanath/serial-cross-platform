# Serial Cross-Platform Library

A lightweight, cross-platform C++ library for serial communication that works on Windows, Linux, and macOS.

## Features

- **Cross-platform**: Single codebase for Windows, Linux, and macOS
- **Simple API**: Easy-to-use interface for serial communication
- **Auto-detection**: Automatically find and connect to available serial ports
- **Manual connection**: Connect to specific ports with custom baud rates
- **Header-only ready**: Minimal dependencies

## Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- Windows: Visual Studio 2017+ or MinGW
- Linux: GCC 7+ or Clang 5+
- macOS: Xcode 9+ or Clang 5+

## Building the Library

### Prerequisites

Make sure you have CMake installed on your system:
- **Windows**: Download from [cmake.org](https://cmake.org/download/) or use `choco install cmake`
- **Linux**: `sudo apt-get install cmake` (Ubuntu/Debian) or `sudo yum install cmake` (CentOS/RHEL)
- **macOS**: `brew install cmake` or download from cmake.org

### Build Instructions

1. **Clone or download the source code**
   ```bash
   git clone <repository-url>
   cd serial-cross-platform
   ```

2. **Create a build directory**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure the project**
   ```bash
   cmake ..
   ```

4. **Build the library and example**
   ```bash
   cmake --build .
   ```

5. **Optional: Install the library**
   ```bash
   cmake --install .
   ```

### Build Options

You can customize the build with these CMake options:

```bash
# Build with specific generator (Windows)
cmake -G "Visual Studio 16 2019" ..

# Build with specific compiler (Linux/macOS)
cmake -DCMAKE_CXX_COMPILER=g++-9 ..

# Build in Release mode
cmake -DCMAKE_BUILD_TYPE=Release ..

# Enable testing (if tests are added)
cmake -DBUILD_TESTING=ON ..
```

## Usage

### Basic Example

```cpp
#include "serial.h"
#include <stdio.h>

int main() {
    Serial serial;
    
    // Auto-connect to the first available serial port
    if (serial.autoconnect(115200)) {
        printf("Successfully connected to serial port!\n");
        
        // Send data
        uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
        int bytes_written = serial.write(data, 4);
        printf("Wrote %d bytes\n", bytes_written);
        
        // Read data
        uint8_t buffer[256];
        int bytes_read = serial.read(buffer, 256);
        if (bytes_read > 0) {
            printf("Read %d bytes\n", bytes_read);
        }
        
        // Connection automatically closed in destructor
    } else {
        printf("Failed to connect to any serial port\n");
    }
    
    return 0;
}
```

### Manual Connection Example

```cpp
#include "serial.h"

void example_with_specific_port() {
    Serial serial;
    
    // Connect to a specific port
    #ifdef _WIN32
    if (serial.connect("COM3", 9600))  // Windows
    #else
    if (serial.connect("/dev/ttyUSB0", 9600))  // Linux/macOS
    #endif
    {
        printf("Connected to specific port\n");
        
        // Use the serial connection...
        
        // Manually disconnect if needed
        serial.disconnect();
    }
}
```

### Status Checking Example

```cpp
#include "serial.h"

void example_status_check() {
    Serial serial;
    
    if (serial.autoconnect(115200)) {
        if (serial.connected()) {
            printf("Serial is connected at %lu baud\n", serial.get_baud_rate());
        }
        
        // Do some work...
        
        // Check if still connected
        if (serial.connected()) {
            printf("Still connected\n");
        }
    }
}
```

## API Reference

### Constructor/Destructor
- `Serial()` - Create a new Serial object
- `~Serial()` - Automatically disconnect and cleanup

### Connection Methods
- `bool autoconnect(unsigned long baud_rate)` - Auto-detect and connect to first available port
- `bool connect(const char* port_name, unsigned long baud_rate)` - Connect to specific port
- `void disconnect()` - Manually disconnect

### Data Transfer
- `int write(uint8_t* data, int size)` - Write data to serial port
- `int read(uint8_t* buffer, int buffer_size)` - Read data from serial port

### Status Methods
- `bool connected()` - Check if currently connected
- `unsigned long get_baud_rate()` - Get current baud rate

## Platform-Specific Information

### Windows
- **Port naming**: `COM1`, `COM2`, etc. (uses `\\.\COM1` internally)
- **Auto-detection**: Searches COM1 through COM255
- **Dependencies**: Windows API (no additional libraries needed)

### Linux
- **Port naming**: `/dev/ttyUSB0`, `/dev/ttyACM0`, `/dev/ttyS0`, etc.
- **Auto-detection**: Searches `/dev/ttyUSB0` through `/dev/ttyUSB255`
- **Dependencies**: pthread library
- **Permissions**: May need to add user to `dialout` group: `sudo usermod -a -G dialout $USER`

### macOS
- **Port naming**: `/dev/tty.usbserial-*`, `/dev/tty.usbmodem*`, etc.
- **Auto-detection**: Searches `/dev/ttyUSB0` through `/dev/ttyUSB255`
- **Dependencies**: pthread library

## Troubleshooting

### Common Issues

1. **Permission denied (Linux/macOS)**
   ```bash
   # Add user to dialout group (Linux)
   sudo usermod -a -G dialout $USER
   # Log out and back in, or run:
   newgrp dialout
   ```

2. **Port not found**
   - Check if device is properly connected
   - Verify port name is correct for your platform
   - Try using `autoconnect()` to find available ports

3. **Build errors**
   - Ensure CMake version is 3.10 or higher
   - Check that C++17 compiler is available
   - On Windows, make sure you have Visual Studio or MinGW installed

### Debugging

Enable verbose output during build:
```bash
cmake --build . --verbose
```

## License

[Add your license information here]

## Contributing

[Add contribution guidelines here] 