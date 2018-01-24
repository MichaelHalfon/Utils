//
// Created by Micky on 1/23/2018.
//

#include "WindowsSockets.hpp"

namespace mutils::net::udp {

    WindowsSockets::WindowsSockets() : _infos{}, _sizeInfo(sizeof(_infos)) {
        struct protoent *proto = getprotobyname("UDP");
        int flag = 1;

        if ((_socket = socket(AF_INET, SOCK_DGRAM, proto->p_proto)) == -1 ||
            setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&flag), sizeof(int)) == -1)
            throw std::runtime_error(msg::errorCreationSocket);
    }

    void WindowsSockets::bind(int port) {
        sockaddr_in sin;

        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

        if (::bind(_socket, (sockaddr *) &sin, sizeof(sin)) == -1)
            throw std::runtime_error(msg::errorBindSocket);
    }

    void WindowsSockets::setServerInformations(const std::string &, int port) {
        _port = port;
        _infos.sin_addr.s_addr = INADDR_ANY;
        _infos.sin_port = htons(port);
        _infos.sin_family = AF_INET;
    }

    ssize_t WindowsSockets::sendData(const char *msg, size_t length) const {
        ssize_t size = sendto(_socket, msg, length, 0, (sockaddr *)&_infos, _sizeInfo);

        return size;
    }

    DataInfos WindowsSockets::receiveData(char * msg, size_t length) const {
        SOCKADDR_IN peeraddr;
        int peeraddrlen = sizeof(peeraddr);

        auto buffer = new char[length];
        ssize_t size = recvfrom(_socket, ((msg == nullptr)? buffer : msg), length, 0, (sockaddr *)&_infos, (socklen_t *)&_sizeInfo);

        if (size <= 0)
            return { };

        DataInfos info;

        getpeername(_socket, (SOCKADDR *)&peeraddr, &peeraddrlen);
        InetNtop(AF_INET, &peeraddr.sin_addr, buffer, INET_ADDRSTRLEN);
        info.ipAddress = std::string(buffer);
        info.data = std::string(((msg == nullptr)? buffer : msg));

        delete buffer;

        return info;
    }

}
