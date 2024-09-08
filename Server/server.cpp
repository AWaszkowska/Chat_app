#include "server.h"

// the server will bind to this endpoint and listen for connections on the specified ports
server::server(asio::io_context& io_context, short port)
    : m_acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    accept();
}

void server::accept()
{
    // a callback gets invoked when a new connection is accepted
    m_acceptor.async_accept(
        [this](std::error_code err, asio::ip::tcp::socket socket) {
            if(!err)
                {
                std::make_shared<session>(std::move(socket))->start();
            }
            accept();
        });
}

void server::addClient(const std::string& ip, unsigned short port)
{
    clients[ip] = ClientsInfo{port, true};
}

void server::removeClient(const std::string& ip, unsigned short port)
{
    auto it = clients.find(ip);
    if (it != clients.end() && it->second.port == port) {
        clients.erase(it);
    }
}

void server::checkStatus() {
    while (true) {
        for (auto& [ip, clientInfo] : clients) { // Using structured binding
            bool connected = dev.isConnected(ip, clientInfo.port);
            if (connected != clientInfo.isOnline) {
                clientInfo.isOnline = connected;
                if (connected) {
                    std::cout << "Client " << ip << ":" << clientInfo.port << " is online." << std::endl;
                } else {
                    std::cout << "Client " << ip << ":" << clientInfo.port << " is offline." << std::endl;
                }
            }
        }
    }
}

