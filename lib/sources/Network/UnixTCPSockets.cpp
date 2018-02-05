//
// Created by mboublil on 12/1/17.
//

#include <stdexcept>
#include <netdb.h>
#include <memory>
#include <cstring>
#include "UnixSockets.hpp"

namespace mutils::net::tcp {

    UnixSockets::UnixSockets() {
        struct protoent *proto = getprotobyname("TCP");
        int flag = 1;

        if ((_socket = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1 ||
            setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1)
            throw std::runtime_error(msg::errorCreationSocket);
    }

    UnixSockets::UnixSockets(int fd) {
        _socket = fd;
        sockaddr_in peeraddr;
        socklen_t peeraddrlen = sizeof(peeraddr);

        getpeername(_socket, (sockaddr *)&peeraddr, &peeraddrlen);
        _ipAddress = inet_ntoa(peeraddr.sin_addr);
    }

    void UnixSockets::bind(int port) {
        sockaddr_in sin;

        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

        if (::bind(_socket, (sockaddr *) &sin, sizeof(sin)) == -1)
            throw std::runtime_error(msg::errorBindSocket);
    }

    void UnixSockets::listen() {
        if (::listen(_socket, tcp::maxClientQueue) == -1)
            throw std::runtime_error(msg::errorListenSocket);
    }

    ITCPSocket * UnixSockets::accept() {
        sockaddr_in csin;
        int sinsize = sizeof(csin);
        int csock;

        if ((csock = ::accept(_socket, (sockaddr *) &csin, (socklen_t *) &sinsize)) == -1)
            throw std::runtime_error(msg::errorAcceptClient);

        ITCPSocket *newSocket = new UnixSockets(csock);

        return (newSocket);
    }

    void UnixSockets::setServerInformations(const std::string &hostname, int port) {
        _hostname = hostname;
        _port = port;
    }

    void UnixSockets::connect() {
        struct hostent *hostinfo = nullptr;

        hostinfo = gethostbyname(_hostname.c_str());
        if (hostinfo == nullptr)
            throw std::runtime_error(msg::errorUnknownHostname);

        _infos.sin_addr = *(in_addr *) hostinfo->h_addr;
        _infos.sin_port = htons(_port);
        _infos.sin_family = AF_INET;

        if (::connect(_socket, (sockaddr *) &_infos, sizeof(sockaddr)) == -1)
            throw std::runtime_error(msg::errorConnectionServer);
    }

    void UnixSockets::connect(std::string const &hostname, int port) {
        struct hostent *hostinfo = nullptr;

        hostinfo = gethostbyname(hostname.c_str());
        if (hostinfo == nullptr)
            throw std::runtime_error(msg::errorUnknownHostname);

        _infos.sin_addr = *(in_addr *) hostinfo->h_addr;
        _infos.sin_port = htons(port);
        _infos.sin_family = AF_INET;

        if (::connect(_socket, (sockaddr *) &_infos, sizeof(sockaddr)) == -1)
            throw std::runtime_error(msg::errorConnectionServer);
    }

    void UnixSockets::disconnect() {
        if (_socket != -1)
            if (close(_socket) == -1)
                throw std::runtime_error(msg::errorCloseSocket);
        _socket = -1;
    }

    ssize_t UnixSockets::sendData(const char *msg, size_t length) const {
        ssize_t size = send(_socket, msg, length, 0);

        return size;
    }

    DataInfos UnixSockets::receiveData(char *msg, size_t length) const {
        char buffer[length];
        ssize_t size = recv(_socket, ((msg == nullptr)? buffer : msg), length, 0);

        if (size != length)
            return { };

        DataInfos info;

        info.senderId = _socket;
        info.size = size;
        info.data = std::string(((msg == nullptr)? buffer : msg), length);
        return info;
    }
}
