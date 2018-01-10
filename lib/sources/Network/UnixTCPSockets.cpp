//
// Created by mboublil on 12/1/17.
//

#include <stdexcept>
#include <netdb.h>
#include <memory>
#include <cstring>
#include "UnixSockets.hpp"

namespace mutils {
    namespace net {
        namespace tcp {

            UnixSockets::UnixSockets() {
                struct protoent *proto = getprotobyname("TCP");
                int flag = 1;

                if ((_socket = socket(AF_INET, SOCK_STREAM, proto->p_proto)) == -1 ||
                    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1)
                    throw std::runtime_error(msg::errorCreationSocket);
            }

            UnixSockets::UnixSockets(int fd) : _socket(fd) {
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

            std::unique_ptr<mutils::net::ITCPSocket> UnixSockets::accept() {
                sockaddr_in csin;
                int sinsize = sizeof(csin);
                int csock;

                if ((csock = ::accept(_socket, (sockaddr *) &csin, (socklen_t *) &sinsize)) == -1)
                    throw std::runtime_error(msg::errorAcceptClient);

                std::unique_ptr<mutils::net::ITCPSocket> newSocket(new UnixSockets(csock));
                return (newSocket);
            }

            void UnixSockets::setServerInformations(const std::string &hostname, int port) {
                struct hostent *hostinfo = nullptr;

                sockaddr_in sin;

                hostinfo = gethostbyname(hostname.c_str());
                if (hostinfo == nullptr)
                    throw std::runtime_error(msg::errorUnknownHostname);

                sin.sin_addr = *(in_addr *) hostinfo->h_addr;
                sin.sin_port = htons(port);
                sin.sin_family = AF_INET;

                if (::connect(_socket, (sockaddr *) &sin, sizeof(sockaddr)) == -1)
                    throw std::runtime_error(msg::errorConnectionServer);
            }

            ssize_t UnixSockets::sendData(const std::stringstream &is, size_t length) {
                ssize_t size = send(_socket, is.str().c_str(), length, 0);

                return size;
            }

            clientInfo UnixSockets::receiveData(std::stringstream &os, size_t length) {
                char buffer[length];
                ssize_t size = recv(_socket, buffer, length, 0);

                if (size <= 0)
                    return { };

                clientInfo info;

                os.write(buffer, length);

                info.id = _socket;
                info.size = size;
                info.data = buffer;
                return info;
            }
        }
    }
}
