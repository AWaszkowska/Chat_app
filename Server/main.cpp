#include <iostream>
#include <asio.hpp>
#include <iostream>
#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    try {
        // Default port value
        short port = 8080;

        if (argc == 2) {
            port = std::atoi(argv[1]);  // Use provided port from command-line argument
        } else if (argc != 1) {
            std::cerr << "Usage: ServerApp [port]" << std::endl;
            return 1;
        }

        asio::io_context io_context;
        server s(io_context, port);
        io_context.run();
    }
    catch (std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
     }

    return 0;
}
