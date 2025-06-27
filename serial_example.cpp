#include "serial.h"
#include <stdio.h>

int main() {
    // Create a Serial object
    Serial serial;
    
    // Auto-connect to the first available serial port
    if (serial.autoconnect(115200)) {
        printf("Successfully connected to serial port!\n");
        
        // Example: Send some data
        uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
        int bytes_written = serial.write(data, 4);
        printf("Wrote %d bytes\n", bytes_written);
        
        // Example: Read some data
        uint8_t buffer[256];
        int bytes_read = serial.read(buffer, 256);
        if (bytes_read > 0) {
            printf("Read %d bytes\n", bytes_read);
        }
        
        // The destructor will automatically close the connection
    } else {
        printf("Failed to connect to any serial port\n");
    }
    
    return 0;
}

// Alternative usage example:
void example_with_specific_port() {
    Serial serial;
    
    // Connect to a specific port
    if (serial.connect("COM3", 9600))  // On Windows
    // if (serial.connect("/dev/ttyUSB0", 9600))  // On Linux
    {
        printf("Connected to specific port\n");
        
        // Use the serial connection...
        
        // Manually disconnect if needed
        serial.disconnect();
    }
}

// Simple example showing status methods
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