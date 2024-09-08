#ifndef DEVICE_H
#define DEVICE_H
#include <iostream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif




class device
{
public:
    device();

    bool isConnected(const std::string ip, unsigned short port);
    void do_sockets();
    void clear_sockets();
};

#endif // DEVICE_H
