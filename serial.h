#pragma once

#include <cstdint>
#include <string>
#include "bytestream.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class Serial 
{

public:
    Serial();
    ~Serial();

	typedef bool (*frame_cb_t)(bytestream_t * periph, void* user_context);
    int register_callback(frame_cb_t cb, void * user_context = NULL);


    // Main interface methods
    bool autoconnect(unsigned long baudrate);
    bool connect(const char* port_name, unsigned long baudrate);
    void disconnect();

    // Read/Write methods
    int write(uint8_t* data, int size);
    int read(uint8_t* buffer, int buffer_size);
    int read_until_delimiter(uint8_t* buffer, size_t buffer_size, unsigned char delimiter, int timeout_ms);
	int read_until_valid(unsigned char * buffer, size_t buffer_size, unsigned char delimiter, int timeout_ms);

    // Status methods
    bool connected();
    unsigned long get_baud_rate();

    // Platform-specific port enumeration (optional)
    static const char* find_first_available_port();

private:
#ifdef _WIN32
    HANDLE serial_handle;
#else
    int serial_fd;
#endif

	enum {
		ERROR_DISCONNECTED = -1,
		ERROR_READ_TIMEOUT = -2,
		ERROR_READ_FAILURE = -3,
		ERROR_UNREGISTERED_CALLBACK = -4,
		ERROR_INVALID_INPUT = -5
	};

    bool is_connected;
    unsigned long baud_rate;

    // Platform-specific connection methods
    bool connect_to_port(const char* port_name);
    void configure_port();

	void * _user_context = NULL;
	frame_cb_t _frame_found_callback = NULL;

}; 