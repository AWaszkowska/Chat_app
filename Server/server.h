#ifndef SERVER_H
#define SERVER_H
#include <asio.hpp>
#include "session.h"
#include <chrono>
#include "device.h"
#include <unordered_map>

struct ClientsInfo {
    unsigned short port;
    bool isOnline;
};

class server
{
public:
    server(asio::io_context& io_context, short port);
private:
    void accept();
    void addClient(const std::string& ip, unsigned short port);
    void removeClient(const std::string& ip, unsigned short port);
    void checkStatus();

    asio::ip::tcp::acceptor m_acceptor;
    std::unordered_map<std::string, ClientsInfo> clients;
    device dev;
};

#endif // SERVER_H
