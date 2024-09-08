#ifndef CLIENT_H
#define CLIENT_H
#include <asio.hpp>
#include <deque>
#include <iostream>

class client
{
public:
    //endpoints that were resolvedfrom the server address
    client(asio::io_context& io_context, const asio::ip::tcp::resolver::results_type& endpoints);

    void write(const std::string& msgs);

private:
    void connect(const asio::ip::tcp::resolver::results_type& endpoints);
    void read();
    void do_write();

    asio::ip::tcp::socket m_socket;
    std::string m_read_msg;
    std::deque<std::string> m_write_msg;
};

#endif // CLIENT_H
