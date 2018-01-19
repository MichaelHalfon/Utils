//
// Created by mboublil on 1/18/18.
//

#include "AsyncSocket.hpp"

namespace mutils::net {
    AsyncSocket::AsyncSocket() : _lk(_mutexCv) {
        _clientThread = std::thread([this]() {
            while (!_stop) {

                _cv.wait(_lk, [](int ));
//                analyzeSockets();
//
//                for (auto &sock : _sockets) {
//                    if (FD_ISSET(sock->getSocket(), &_rfds)) {
//                        std::cout << "content to read" << std::endl;
//                        readContent(sock.get());
//                    }
//                }
//
//                while (hasToSend()) {
//                    std::pair<int, BinaryData> msgToSend = toSend();
//                    auto clientId = msgToSend.first;
//
//                    auto it = std::find_if(_sockets.begin(), _sockets.end(), [this, &clientId](std::shared_ptr<ITCPSocket> const &s1) {
//                        return s1->getSocket() == clientId;
//                    });
//                    (*it)->sendData(msgToSend.second._data.c_str(), msgToSend.second._data.size());
//                }
            }
        });
    }

    AsyncSocket::~AsyncSocket() {
        _stop = true;
        _clientThread.join();
    }

    void AsyncSocket::analyzeSockets() {
        timeval t;

        t.tv_sec = 1;
        t.tv_usec = 0;

        initSockets();
        select((int)_sockets.size() + 1, &_rfds, nullptr, nullptr, &t);
    }

    void AsyncSocket::initSockets() {
        FD_ZERO(&_rfds);
        FD_SET(3, &_rfds);
        for (auto &socket : _sockets) {
            FD_SET(socket->getSocket(), &_rfds);
        }
    }

    bool AsyncSocket::assign(std::shared_ptr<ITCPSocket> const &sock) {
        if ((int)getNbConnections() >= nbMaxConnections)
            return false;
        FD_SET(sock->getSocket(), &_rfds);
        _sockets.push_back(sock);
        return true;
    }

    Header AsyncSocket::getHeader(ITCPSocket const *sock) {
        std::size_t size;
        std::uint16_t type;

        sock->receiveData(reinterpret_cast<char *>(&type), sizeof(std::uint16_t));
        sock->receiveData(reinterpret_cast<char *>(&size), sizeof(std::size_t));

        return {type, size};
    }

    void AsyncSocket::readContent(ITCPSocket const *sock) {
        Header hdr = getHeader(sock);
        DataInfos infos = sock->receiveData(nullptr, hdr.size);
        BinaryData data;

        data.type = hdr.type;
        data._data = infos.data;
        std::cout << "content: " << data._data << std::endl;
        received(sock->getSocket(), data);
    }

}
