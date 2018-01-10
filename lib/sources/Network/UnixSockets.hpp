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
#include <iostream>
#include "ISocket.hpp"

namespace mutils {
    namespace net {
        namespace tcp {

            const int maxClientQueue = 128;

            class UnixSockets : public ITCPSocket {
            public:
                UnixSockets();
                ~UnixSockets() override { close(_socket); }
                explicit UnixSockets(int fd);

                void bind(int port) override;
                void listen() override;
                std::unique_ptr<ITCPSocket> accept() override;
                void setServerInformations(const std::string &hostname, int port) override;
                ssize_t sendData(const std::stringstream &is, size_t length) override;
                clientInfo receiveData(std::stringstream &os, size_t length) override;

            private:
                int _socket;
                int _port;
                std::string _hostname;
                std::string _ipAddress;
            };
        }

        namespace udp {
            class UnixSockets : public IUDPSocket {
            public:
                UnixSockets();
                ~UnixSockets() override { close(_socket); }

                void bind(int port) override;

                void setServerInformations(const std::string &hostname, int port) override;

                ssize_t sendData(const std::stringstream &is, size_t length) override;

                clientInfo receiveData(std::stringstream &os, size_t length) override;

            private:
                sockaddr_in _infos;
                int _socket;
                int _sizeInfo;
            };
        }
        namespace msg {
            const std::string errorCreationSocket = "Error during the creation of the socket.";
            const std::string errorBindSocket = "Error during the binding of the socket.";
            const std::string errorListenSocket = "Error during the listening on the socket.";
            const std::string errorAcceptClient = "Error during the accepting of a new client.";
            const std::string errorUnknownHostname = "Error: unknown hostname.";
            const std::string errorConnectionServer = "Error during the connection on the server.";
        }
    }
}

#endif //MUTILS_UNIXSOCKETS_HPP
