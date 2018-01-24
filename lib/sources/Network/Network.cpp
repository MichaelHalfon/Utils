//
// Created by mboublil on 1/9/18.
//

#include <memory>
#include <cstdint>
#include "Network.hpp"
#include "events.hpp"

namespace mutils::net {

    Network::Network(bool isServer)
            : _tcpConnection(new TCPSocket()), _isServer(isServer) {
        if (isServer) {
            name = "Server";
        }
        else
            name = "Client";
    }

    //TODO: Vérifier que ça ne pete pas en multi - client
    void Network::init() {
        ++_phase;

        addReaction<fender::events::Shutdown>([this](futils::IMediatorPacket &) {
            _serverShutdown = true;
            _listenerThread.join();
            entityManager->removeSystem(name);
        });

        if (_isServer) {
            _tcpConnection->bind(4242);
            _tcpConnection->listen();
            _listenerThread = std::thread([this]() {
                while (!_serverShutdown) {
                    timeval t;

                    FD_ZERO(&_rfds);
                    FD_SET(_tcpConnection->getSocket(), &_rfds);
                    for (auto &sock : _connections) {
                        FD_SET(sock, &_rfds);
                    }
                    t.tv_sec = 1;
                    t.tv_usec = 0;
                    select(_tcpConnection->getSocket() + 1, &_rfds, nullptr, nullptr, &t);
                    std::cout << "Apres le select" << std::endl;
                    if (FD_ISSET(_tcpConnection->getSocket(), &_rfds)) {
                        std::cout << "new client !" << std::endl;
                        auto conn = _tcpConnection->accept();
                        FD_SET(conn->getSocket(), &_rfds);
                        _connectionSocket[conn->getSocket()] = new AsyncSocket(conn);
                        _connections.push_back(conn->getSocket());
                    }
                    for (auto &sock : _connections) {
                        if (FD_ISSET(sock, &_rfds)) {
                            std::cout << "There is something to read !" << std::endl;
                            _fds.push_back(sock);
                            _cv.notify_all();
//                            std::unique_lock<std::mutex> lk(_mutexCv);
//                            _cv.wait(lk, [] {return dataProcessed;});
                            std::cout << "size: " << _fds.size() << std::endl;
                        }
                    }
                }
            });
        }
        else {
            std::cout << "Connecting to server..." << std::endl;
            _tcpConnection->setServerInformations("localhost", 4242);
            _tcpConnection->connect();
            std::cout << "Connected !" << std::endl;

            std::stringstream ss;
            std::uint16_t t = 0x0001;
            std::size_t s = sizeof(test);
            test ts;

            ss.write(reinterpret_cast<char *>(&t), sizeof(std::uint16_t));
            ss.write(reinterpret_cast<char *>(&s), sizeof(std::size_t));
            ss << ts;

            _tcpConnection->sendData(ss.str().c_str(), sizeof(test));
        }
    }

    void Network::monitorConnections() {
        for (auto &item : _connectionSocket) {
            if(item.second->hasReceived()) {
                Packet pkg;

                std::pair<SOCKET, BinaryData> receivedElem = item.second->read();
                pkg.clientId = receivedElem.first;
                pkg.data = receivedElem.second;

                std::cout << pkg.data._data << std::endl;
                events->send<Packet>(pkg);
            }
        }
    }

    void Network::run(float elapsed) {
        _sumTime += elapsed;
        if (_isServer || (!_isServer && _sumTime >= 2)) {
            _sumTime = 0;
            switch (_phase) {
                case 0 :
                    return init();
                default :
                    if (_isServer) return monitorConnections();
            }
        }
    }
}
