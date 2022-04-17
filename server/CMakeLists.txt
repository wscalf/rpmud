cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

project(rpmud VERSION 0.1)
configure_file(version.h.in version.h)

add_executable(rpmud Main.cpp)

target_link_libraries(rpmud
    PUBLIC 
        pthread
        telnet #move to network, requires libtelnet install
        event #may require libevent install - libevent was already on my system, but the headers were not. There is also a libevent_core that may have all we need
    )

add_subdirectory(util)
add_subdirectory(network)