//
// Created by mboublil on 1/18/18.
//

#include "AsyncSocket.hpp"

namespace mutils::net {

    std::mutex _mutexCv;
    std::vector<int> _fds;
    std::condition_variable _cv;
    std::unordered_map<SOCKET, Actions> _action;
    dataProcessed _process { };
    bool _notified { false };

    AsyncSocket::AsyncSocket(ITCPSocket *sock)  : _sock(sock) {
        _clientThread = std::thread([this]() {
            std::unique_lock<std::mutex> lk(_mutexCv);
            while (!_stop) {
                while (!_notified) {
                    _cv.wait(lk, [this]() {
                        {
                            for (auto it = _fds.begin(); it != _fds.end(); ++it) {
                                if (*it == _sock->getSocket()) {
                                    _fds.erase(it);
                                    return true;
                                } else if (*it == -1)
                                    return true;
                            }
                        }
                        return false;
                    });
                }

                _notified = false;

                switch (_action[_sock->getSocket()]) {
                    case Actions::READ:
                        readContent();
                        _action.erase(_sock->getSocket());
                        break ;
                    case Actions::WRITE:
                        writeContent();
                        _action.erase(_sock->getSocket());
                        break ;
                    default:
                        break ;
                }
                _process.id = _sock->getSocket();
                _process._isFinished = true;
                _cv.notify_all();
            }
        });
    }

    AsyncSocket::~AsyncSocket() {
        _stop = true;
        _fds.push_back(_sock->getSocket());
        _action[_sock->getSocket()] = Actions::NONE;
        _notified = true;
        _cv.notify_all();
        _clientThread.join();
    }

    Header AsyncSocket::getHeader() {
        std::uint16_t type;
        std::size_t size;
        DataInfos infos;


        infos = _sock->receiveData(reinterpret_cast<char *>(&type), sizeof(std::uint16_t));
        if (infos.size == 0)
            return {0, 0};
        infos = _sock->receiveData(reinterpret_cast<char *>(&size), sizeof(std::size_t));
        if (infos.size == 0)
            return {0, 0};

        return {type, size};
    }

    void AsyncSocket::readContent() {
        Header hdr = getHeader();
        if (hdr.size == 0) {
            _process.status = Status::SOCKET_CLOSE;
            return;
        }

        DataInfos infos = _sock->receiveData(nullptr, hdr.size);
        if (infos.size == 0) {
            _process.status = Status ::SOCKET_CLOSE;
            return ;
        }
        PacketReceived data;

        data._size = hdr.size;
        data._type = hdr.type;
        data._data = infos.data;
        data._id = infos.senderId;

        received(data);
    }

    void AsyncSocket::writeContent() {
        auto data = toSend();

        std::cout << "[" << _sock->getSocket() << "]: " << "DATA SENT: " << data._data << std::endl;
        _sock->sendData(data._data.c_str(), data._size);
    }

}
