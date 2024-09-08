cmake_minimum_required(VERSION 3.5)

project(network_chat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add the ASIO include directory
include_directories(C:/Asio/asio-1.30.2/include)

# Find Qt libraries
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    client.h
    client.cpp
)

# Check the Qt version to create the appropriate target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(network_chat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(network_chat SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(network_chat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link Qt libraries
target_link_libraries(network_chat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Link ASIO-related libraries (on Windows)
if (WIN32)
    target_link_libraries(network_chat PRIVATE Ws2_32 Mswsock)
endif()

# Set properties for macOS bundles
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.network_chat)
endif()
set_target_properties(network_chat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Installation settings
include(GNUInstallDirs)
install(TARGETS network_chat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize the executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(network_chat)
endif()
