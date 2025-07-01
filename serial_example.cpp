#include "serial.h"
#include <stdio.h>
/*
Generic 2's complement hex checksum calculation.
Used in the psyonic API
*/
uint8_t get_checksum(uint8_t* arr, int size)
{

    int8_t checksum = 0;
    for (int i = 0; i < size; i++)
        checksum += (int8_t)arr[i];
    return -checksum;
}

int main()
{
    // Create a Serial object
    Serial serial;
    
    // Auto-connect to the first available serial port
    //if (serial.autoconnect(460800)) 
    if(serial.connect("COM5",460800))
    {
                
        // Example: Send some data
        uint8_t data[7] = {'`~', 0x50, 0x77, 'R', 'c', 0x0, '~'};
        data[5] = get_checksum(&data[1], 4);
        int bytes_written = serial.write(data, 7);
        printf("Wrote %d bytes\n", bytes_written);

        uint8_t buffer[256] = {};
        int bytes_read = 0;
        while(bytes_read == 0)
        {
        // Example: Read some 
            bytes_read = serial.read(buffer, 256);
        }
        if (bytes_read > 0) 
        {
            printf("Read %d bytes: %s\n", bytes_read, buffer);
        }

        // The destructor will automatically close the connection
    } 
    else 
    {
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