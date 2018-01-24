//
// Created by mboublil on 1/18/18.
//

#include "AsyncSocket.hpp"

namespace mutils::net {

    std::mutex _mutexCv;
    std::vector<int> _fds;
    std::condition_variable _cv;
    bool dataProcessed = false;

    AsyncSocket::AsyncSocket(std::shared_ptr<mutils::net::ITCPSocket> const &sock)  : _lk(_mutexCv), _sock(sock) {
        _clientThread = std::thread([this]() {
            while (!_stop) {

                _cv.wait(_lk, [this]() {
                    for (auto it = _fds.begin(); it != _fds.end(); it++) {
                        if (*it == _sock->getSocket()) {
                            _fds.erase(it);
                            return true;
                        }
                    }
                    return false;
                });

                readContent();
//                dataProcessed = true;
//                _cv.notify_all();
//                analyzeSockets();
            }
        });
    }

    AsyncSocket::~AsyncSocket() {
        _stop = true;
        _clientThread.join();
    }

    Header AsyncSocket::getHeader() {
        std::size_t size;
        std::uint16_t type;

        _sock->receiveData(reinterpret_cast<char *>(&type), sizeof(std::uint16_t));
        _sock->receiveData(reinterpret_cast<char *>(&size), sizeof(std::size_t));

        return {type, size};
    }

    void AsyncSocket::readContent() {
        Header hdr = getHeader();
        DataInfos infos = _sock->receiveData(nullptr, hdr.size);
        BinaryData data;

        data.type = hdr.type;
        data._data = infos.data;
        std::cout << "content read: " << data._data << std::endl;
        received(_sock->getSocket(), data);
    }

}
