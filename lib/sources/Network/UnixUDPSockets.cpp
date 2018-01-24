//
// Created by mboublil on 12/1/17.
//

#include <stdexcept>
#include <netdb.h>
#include <memory>
#include "UnixSockets.hpp"

namespace mutils::net::udp {

    UnixSockets::UnixSockets() : _infos{}, _sizeInfo(sizeof(_infos)) {
        struct protoent *proto = getprotobyname("UDP");
        int flag = 1;

        if ((_socket = socket(AF_INET, SOCK_DGRAM, proto->p_proto)) == -1 ||
            setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1)
            throw std::runtime_error(msg::errorCreationSocket);
    }

    void UnixSockets::bind(int port) {
        sockaddr_in sin;

        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

        if (::bind(_socket, (sockaddr *) &sin, sizeof(sin)) == -1)
            throw std::runtime_error(msg::errorBindSocket);
    }

    void UnixSockets::setServerInformations(const std::string &hostname, int port) {
        _hostname = hostname;
        _port = port;

        _infos.sin_addr.s_addr = INADDR_ANY;
        _infos.sin_port = htons(port);
        _infos.sin_family = AF_INET;
    }

    ssize_t UnixSockets::sendData(const char *msg, size_t length) const {
        ssize_t size = sendto(_socket, msg, length, 0, (sockaddr *)&_infos, _sizeInfo);

        return size;
    }

    DataInfos UnixSockets::receiveData(char * msg, size_t length) const {
        char buffer[length];
        ssize_t size = recvfrom(_socket, ((msg == nullptr)? buffer : msg), length, 0, (sockaddr *)&_infos, (socklen_t *)&_sizeInfo);

        if (size <= 0)
            return { };

        DataInfos info;

        getpeername(_socket, (sockaddr *)&_infos, (socklen_t *)&_sizeInfo);
        info.ipAddress = inet_ntoa(_infos.sin_addr);
        info.data = std::string(((msg == nullptr)? buffer : msg));

        return info;
    }
}
