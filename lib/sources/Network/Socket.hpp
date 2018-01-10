//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_SOCKET_HPP
#define MUTILS_SOCKET_HPP

#ifdef __linux__

#include "UnixSockets.hpp"

using TCPSocket = mutils::net::tcp::UnixSockets;
using UDPSocket = mutils::net::udp::UnixSockets;

#elif _WIN32

#include "WindowsSockets.hpp"

#endif

#endif //MUTILS_SOCKET_HPP
