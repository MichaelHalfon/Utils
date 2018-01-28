//
// Created by mboublil on 1/27/18.
//

#ifndef FUTILS_NETWORKEVENTS_HPP
#define FUTILS_NETWORKEVENTS_HPP

#include <iostream>
#include "Serializer/Serializable.hpp"
#include "ISocket.hpp"

namespace mutils::net {

    static const std::uint32_t handshakeValue = 0xe3a4c32a;

    struct tryingToConnect {
        std::string hostname;
        int port;
    };

    struct newConnection {
        SOCKET _id;
        std::string _ipAddress;
    };

    struct disconnection {
        SOCKET _id;
    };

    struct error{
        std::string message;
    };

    struct handshake {
        handshake() {}

        std::uint16_t type { 0x0000 };
        std::uint32_t value { handshakeValue };
    };
}

#endif //FUTILS_NETWORKEVENTS_HPP
