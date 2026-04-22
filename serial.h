#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class Serial 
{
private:
#ifdef _WIN32
    HANDLE serial_handle;
#else
    int serial_fd;
#endif
    bool is_connected;
    unsigned long baud_rate;

    // Platform-specific connection methods
    bool connect_to_port(const char* port_name);
    void configure_port();

public:
    Serial();
    ~Serial();
    
    // Main interface methods
    bool autoconnect(unsigned long baudrate);
    bool connect(const char* port_name, unsigned long baudrate);
    void disconnect();
    
    // Read/Write methods
    int write(uint8_t* data, int size);
    int read(uint8_t* buffer, int buffer_size);
    int read_until_delimiter(uint8_t* buffer, size_t buffer_size, uint8_t delimiter, int timeout_ms);
    
    // Status methods
    bool connected();
    unsigned long get_baud_rate();
    
    // Platform-specific port enumeration (optional)
    static const char* find_first_available_port();
}; 