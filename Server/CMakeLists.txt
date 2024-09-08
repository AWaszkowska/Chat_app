cmake_minimum_required(VERSION 3.5)

project(Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the ASIO include directory
include_directories(C:/Asio/asio-1.30.2/include)

add_executable(Server main.cpp
    server.cpp server.h
    session.cpp session.h
    device.cpp device.h)

# Link ASIO-related libraries (on Windows)
if (WIN32)
    target_link_libraries(Server PRIVATE Ws2_32 Mswsock)
endif()

include(GNUInstallDirs)
install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
