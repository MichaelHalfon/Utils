//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_WINDOWSSOCKETS_HPP
#define MUTILS_WINDOWSSOCKETS_HPP

#include <Ws2tcpip.h>
#include <utility>
#include "ISocket.hpp"

namespace mutils::net {
    namespace tcp {

        const int maxClientQueue = 128;

        class WindowsSockets : public ITCPSocket {
        public:
            WindowsSockets();
            ~WindowsSockets() override {
                WSACleanup();
                disconnect();
            }

            explicit WindowsSockets(SOCKET fd);

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
        class WindowsSockets : public IUDPSocket {
        public:
            WindowsSockets();
            ~WindowsSockets() override {
                WSACleanup();
                closesocket(_socket);
            }

            void bind(int port) override;

            void setServerInformations(const std::string &hostname, int port) override;

            ssize_t sendData(const char *, size_t length) const override;

            DataInfos receiveData(char *, size_t length) const override;

        private:
            SOCKADDR_IN _infos;
            size_t _sizeInfo;
            std::string _hostname;
            int _port;
        };
    }
}

#endif //MUTILS_WINDOWSSOCKETS_HPP
