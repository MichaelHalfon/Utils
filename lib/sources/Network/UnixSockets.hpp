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

            const int maxClientQueue = 128;

            class UnixSockets : public ISocket {
            public:
                UnixSockets();
                ~UnixSockets() override { close(_socket); }
                explicit UnixSockets(int fd);

                void bind(int port) override;

                void listen() override;

                std::unique_ptr<ISocket> accept() override;

                void connect(const std::string &hostname, int port) override;

                ssize_t sendData(const std::stringstream &is, size_t length) override;

                ssize_t receiveData(std::stringstream &os, size_t length) override;

            private:
                int _socket;
                int _port;
                std::string _hostname;
            };
        }
        namespace msg {
            static const std::string errorCreationSocket = "Error during the creation of the socket.";
            static const std::string errorBindSocket = "Error during the binding of the socket.";
            static const std::string errorListenSocket = "Error during the listening on the socket.";
            static const std::string errorAcceptClient = "Error during the accepting of a new client.";
            static const std::string errorUnknownHostname = "Error: unknown hostname.";
            static const std::string errorConnectionServer = "Error during the connection on the server.";
        }
    }
}

#endif //MUTILS_UNIXSOCKETS_HPP
