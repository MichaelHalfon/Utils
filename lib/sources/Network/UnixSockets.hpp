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

namespace mutils::net {

    namespace tcp {

        const int maxClientQueue = 128;

        class UnixSockets : public ITCPSocket {
        public:
            UnixSockets();
            ~UnixSockets() override { disconnect(); }

            explicit UnixSockets(int fd);

            void connect() override;
            void connect(std::string const &hostname, int port) override;
            void disconnect() override;

            void bind(int port) override;
            void listen() override;

            std::shared_ptr<ITCPSocket> accept() override;
            void setServerInformations(const std::string &hostname, int port) override;

            ssize_t sendData(const char *, size_t length) const override;
            DataInfos receiveData(char *, size_t length) const override;

        private:
            int _port;
            std::string _hostname;
            std::string _ipAddress;
            sockaddr_in _infos;
        };
    }

    namespace udp {
        class UnixSockets : public IUDPSocket {
        public:
            UnixSockets();
            ~UnixSockets() override {close(_socket); }

            void bind(int port) override;

            void setServerInformations(const std::string &hostname, int port) override;

            ssize_t sendData(const char *, size_t length) const override;

            DataInfos receiveData(char *, size_t length) const override;

        private:
            sockaddr_in _infos;
            socklen_t _sizeInfo;
            std::string _hostname;
            int _port;
        };
    }
}

#endif //MUTILS_UNIXSOCKETS_HPP
