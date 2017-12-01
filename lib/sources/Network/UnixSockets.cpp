//
// Created by mboublil on 12/1/17.
//

#include <stdexcept>
#include "UnixSockets.hpp"

mutils::net::tcp::UnixSockets::UnixSockets() {
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error(msg::errorConnectionSocket);
}

bool mutils::net::tcp::UnixSockets::connect() {
    return false;
}

void mutils::net::tcp::UnixSockets::disconnect() {

}

void mutils::net::tcp::UnixSockets::send() {

}

void mutils::net::tcp::UnixSockets::receive() {

}

