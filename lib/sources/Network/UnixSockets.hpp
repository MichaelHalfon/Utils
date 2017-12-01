//
// Created by mboublil on 12/1/17.
//

#ifndef MUTILS_UNIXSOCKETS_HPP
#define MUTILS_UNIXSOCKETS_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ISocket.hpp"

namespace mutils {
    namespace net {
        namespace tcp {
            class UnixSockets : ISocket {

            public:
                UnixSockets();

            public:
                bool connect() override;

                void disconnect() override;

                void send() override;

                void receive() override;

            private:
                int _socket;
            };
        }
        namespace msg {
            static const std::string errorConnectionSocket = "Error during the connection of the socket";
        }
    }
}


#endif //MUTILS_UNIXSOCKETS_HPP
