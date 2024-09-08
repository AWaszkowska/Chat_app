#include "device.h"

device::device() {}

bool device::isConnected(const std::string ip, unsigned short port) {
    do_sockets();

    int sock;
    struct sockaddr_in server;

    // Create socket (address family- eg. IPv4, socket type- tcp, protocol)
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        std::cout << "Could not create socket";
        clear_sockets();
        return false;
    }

#ifdef _WIN32
    unsigned long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

#else
    // Set the socket to non-blocking mode
    fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;

    // convert a u_short from host to big-endian (TCP/IP network byte order)
    server.sin_port = htons(port);

    // Attempt to connect
    int result = connect(sock, (struct sockaddr *)&server, sizeof(server));

#ifdef _WIN32
    if (result < 0 && WSAGetLastError() == WSAEWOULDBLOCK) {
#else
    if (result < 0 && (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK)) {
#endif
        fd_set set;
        struct timeval timeout;

        FD_ZERO(&set);
        FD_SET(sock, &set);

        timeout.tv_sec = 2;  // Set the timeout period (5 seconds)
        timeout.tv_usec = 0;

        // Wait for the socket to be ready to write (connect finished)
        result = select(sock + 1, NULL, &set, NULL, &timeout);
        if (result <= 0) {
            std::cout << "Connection timed out";
            clear_sockets();
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }
#ifdef _WIN32
        int so_error = 0;
        int len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&so_error, &len);  // Cast to (char*)
#else
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);  // No cast needed
#endif
        if (so_error != 0) {
            std::cout << "Connection failed: " << strerror(so_error);
            clear_sockets();
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }
    }
    else if (result < 0) {
        std::cout << "Connection failed: " << strerror(errno);
        clear_sockets();
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

    // Clean up
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    clear_sockets();

    std::cout << "Device is connected " << true;
    return true;
}

void device::do_sockets()
{
#ifdef _WIN32
    WSADATA wsd;
    WSAStartup(MAKEWORD(2,2), &wsd);
#endif
}

void device::clear_sockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}
