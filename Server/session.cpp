#include "session.h"
#include <iostream>

session::session(asio::ip::tcp::socket socket)
    : m_socket(std::move(socket))
{

}

void session::start()
{
    read();
}

void session::deliver(const std::string& msg)
{
    bool write_in_progress = !m_write_msg.empty(); // if deque is empty, false
    m_write_msg.push_back(msg);
    if(!write_in_progress)
        write();
}

void session::read()
{
    auto self(shared_from_this());
    asio::async_read_until(m_socket, asio::dynamic_buffer(m_read_msg), '\n',
                           [this, self](std::error_code err, std::size_t len)
                           {
        if(!err)
            {
            std::cout << "Received: " << m_read_msg << std::endl;
            m_read_msg.clear();
            read();
        }
        else {
            // Handle err
        }
    });
}

void session::write()
{
    auto self(shared_from_this());
    asio::async_write(m_socket, asio::buffer(m_write_msg.front()),
                      [this, self](std::error_code err, std::size_t /*length*/)
                      {
        if(!err)
            {
            m_write_msg.pop_front();
            if(!m_write_msg.empty())
            {
                write();
            }
        }
    });
}
