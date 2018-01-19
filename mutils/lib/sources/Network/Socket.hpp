//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_SOCKET_HPP
#define MUTILS_SOCKET_HPP

#ifdef __linux__

# include "UnixSockets.hpp"

using TCPSocket = mutils::net::tcp::UnixSockets;
using UDPSocket = mutils::net::udp::UnixSockets;
using SOCCKADDR_IN = sockaddr_in;
using SOCKADDR = sockaddr;
using IN_ADDR = in_addr;

#elif _WIN32

#include "WindowsSockets.hpp"

#endif

#endif //MUTILS_SOCKET_HPP
