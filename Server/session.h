#ifndef SESSION_H
#define SESSION_H
#include <asio.hpp>
#include <deque>


class session : public std::enable_shared_from_this<session>
{
public:
    // initialize a session with a socket in a constructor
    session(asio::ip::tcp::socket socket);

    //start session, initialize read operation
    void start();

    // deliver a message to this client
    void deliver(const std::string& msg);

private:
    void read();
    void write();

    asio::ip::tcp::socket m_socket; // socket for this client connection
    std::string m_read_msg; // buffer for incoming messages
    std::deque<std::string> m_write_msg; // queue for outgoing messages
};

#endif // SESSION_H
