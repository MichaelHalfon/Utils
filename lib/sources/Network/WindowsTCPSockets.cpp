//
// Created by Micky on 1/23/2018.
//

#include <iostream>
#include "WindowsSockets.hpp"

namespace mutils::net::tcp {
    WindowsSockets::WindowsSockets() {
        WSADATA wsa;
        int flag = 1;

        if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
            throw std::runtime_error("Error during WSA Startup.");
        if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1 ||
            setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&flag), sizeof(int)) == -1)
            throw std::runtime_error(msg::errorCreationSocket);
    }

    WindowsSockets::WindowsSockets(SOCKET fd) {
        char buffer[INET_ADDRSTRLEN];

        _socket = fd;
        SOCKADDR_IN peeraddr;
        int peeraddrlen = sizeof(peeraddr);

        getpeername(_socket, (SOCKADDR *)&peeraddr, &peeraddrlen);
        InetNtop(AF_INET, &peeraddr.sin_addr, buffer, INET_ADDRSTRLEN);
        _ipAddress = std::string(buffer);
    }

    void WindowsSockets::bind(int port) {
        SOCKADDR_IN sin;

        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

        if (::bind(_socket, (SOCKADDR *) &sin, sizeof(sin)) == -1)
            throw std::runtime_error(msg::errorBindSocket);
    }

    void WindowsSockets::listen() {
        if (::listen(_socket, tcp::maxClientQueue) == -1)
            throw std::runtime_error(msg::errorListenSocket);
    }

    std::shared_ptr<ITCPSocket> WindowsSockets::accept() {
        SOCKADDR_IN csin;
        int sinsize = sizeof(csin);
        int csock;

        if ((csock = ::accept(_socket, (sockaddr *) &csin, &sinsize)) == -1)
            throw std::runtime_error(msg::errorAcceptClient);

        ITCPSocket *newSocket = new UnixSockets(csock);
        return (newSocket);
    }

    void WindowsSockets::setServerInformations(const std::string &hostname, int port) {
        _hostname = hostname;
        _port = port;
    }

    void WindowsSockets::connect() {
        struct addrinfo hostinfo;
        struct addrinfo *result = nullptr;
        struct addrinfo *ptr = nullptr;
        int flag = 1;

        memset(&hostinfo, 0, sizeof(hostinfo));
        hostinfo.ai_family = AF_UNSPEC;
        hostinfo.ai_socktype = SOCK_STREAM;
        hostinfo.ai_protocol = IPPROTO_TCP;

        closesocket(_socket);
        std::cout << _hostname << "; " << _port << std::endl;
        auto val = getaddrinfo(_hostname.c_str(), std::to_string(_port).c_str(), &hostinfo, &result);
        std::cout << val << std::endl;

        for(ptr=result; ptr != nullptr ;ptr = ptr->ai_next) {

            if ((_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1 ||
                setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&flag), sizeof(int)) == -1)
                throw std::runtime_error(msg::errorCreationSocket);
            std::cout << "creation de socket reussi" << std::endl;

            int res = ::connect(_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
            if (res == SOCKET_ERROR) {
                closesocket(_socket);
                continue;
            }
            break;
        }
        std::cout << "fin de boucle" << std::endl;
        if (ptr == nullptr)
            throw std::runtime_error(msg::errorConnectionServer);
        std::cout << "je suis a la fin de la fonction. Du coup ca ne va jamais s'afficher" << std::endl;
        freeaddrinfo(result);
    }

    void WindowsSockets::connect(std::string const &hostname, int port) {
        struct addrinfo hostinfo;
        struct addrinfo *result = nullptr;
        struct addrinfo *ptr = nullptr;
        int flag = 1;

        hostinfo.ai_family = AF_UNSPEC;
        hostinfo.ai_socktype = SOCK_STREAM;
        hostinfo.ai_protocol = IPPROTO_TCP;

        getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hostinfo, &result);

        for(ptr=result; ptr != nullptr ;ptr = ptr->ai_next) {

            if ((_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) ||
                setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&flag), sizeof(int)) == -1)
                throw std::runtime_error(msg::errorCreationSocket);

            int res = ::connect(_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
            if (res == SOCKET_ERROR) {
                closesocket(_socket);
                continue;
            }
            break;
        }
        if (ptr == nullptr)
            throw std::runtime_error(msg::errorConnectionServer);
        freeaddrinfo(result);
    }

    void WindowsSockets::disconnect() {
        if (_socket != -1)
            if (closesocket(_socket) == -1)
                throw std::runtime_error(msg::errorCloseSocket);
        _socket = INVALID_SOCKET;
    }

    ssize_t WindowsSockets::sendData(const char *msg, size_t length) const {
        ssize_t size = send(_socket, msg, length, 0);

        return size;
    }

    DataInfos WindowsSockets::receiveData(char *msg, size_t length) const {
        auto buffer = new char[length];
        ssize_t size = recv(_socket, ((msg == nullptr)? buffer : msg), length, 0);

        if (size <= 0)
            return { };

        DataInfos info;

        info.senderId = _socket;
        info.size = size;
        info.data = std::string(((msg == nullptr)? buffer : msg));
        delete buffer;
        return info;
    }
}
