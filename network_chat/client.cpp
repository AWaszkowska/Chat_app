#include "client.h"

client::client(asio::io_context& io_context, const asio::ip::tcp::resolver::results_type& endpoints)
    : m_socket(io_context)
{
    //initializer list binds the socket to the io_context, enabling async operations
    connect(endpoints);
}

void client::read()
{
    asio::async_read_until(m_socket, asio::dynamic_buffer(m_read_msg), '\n',
                                                          [this](std::error_code err, std::size_t len)
                                                          {
                               if(!err)
                                   {
                                   std::cout << "Received" << m_read_msg;
                                   m_read_msg.clear();
                                   read();
                               }
    });
}

void client::write(const std::string& msgs)
{
    // Post the lambda to the io_context's event loop associated with the socket's executor
    asio::post(m_socket.get_executor(), [this, msgs]() mutable {
        bool write_in_progress = !m_write_msg.empty();  // Check if a write operation is already in progress
        m_write_msg.push_back(msgs + "\n");  // Add the new message to the queue with a newline character

        if (!write_in_progress) {
            do_write();  // If no write operation is in progress, start writing
        }
    });
}


void client::do_write()
{
    asio::async_write(m_socket, asio::buffer(m_write_msg.front()),
                      [this](std::error_code err, std::size_t /*len*/)
                      {
        if(!err){
            m_write_msg.pop_front();
            if(!m_write_msg.empty()){
                do_write();
            }
        }
    });
}

void client::connect(const asio::ip::tcp::resolver::results_type& endpoints)
{
    asio::async_connect(m_socket, endpoints,
                        [this](std::error_code err, asio::ip::tcp::endpoint)
                        {
                            if (!err) {
                                read(); // Start reading once the connection is successful
                            } else {
                                std::cerr << "Connection failed: " << err.message() << std::endl;
                            }
                        });
}
