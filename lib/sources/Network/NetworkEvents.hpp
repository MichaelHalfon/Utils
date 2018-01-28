//
// Created by mboublil on 1/27/18.
//

#ifndef FUTILS_NETWORKEVENTS_HPP
#define FUTILS_NETWORKEVENTS_HPP

#include <iostream>

namespace mutils::net {

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
}

#endif //FUTILS_NETWORKEVENTS_HPP
