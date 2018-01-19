//
// Created by mboublil on 1/9/18.
//

#include <memory>
#include <cstdint>
#include "Network.hpp"
#include "Listener.hpp"
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

    void Network::init() {
        ++_phase;

        addReaction<fender::events::Shutdown>([this](futils::IMediatorPacket &) {
            entityManager->removeSystem(name);
        });

        if (_isServer) {
            entityManager->addSystem<Listener>();

            addReaction<newClientMessage>([this](futils::IMediatorPacket &pkg) {
                std::cout << "New Client !" << std::endl;

                auto msg = futils::Mediator::rebuild<newClientMessage>(pkg);
                auto it = _asyncSockets.begin();

                for (; it != _asyncSockets.end(); it++) {
                    if((*it)->assign(msg.client))
                        break ;
                }

                if (it == _asyncSockets.end()) {
                    std::unique_ptr<AsyncSocket> as(new AsyncSocket);
                    _asyncSockets.push_back(std::move(as));
                    it = _asyncSockets.end();
                    it--;
                    (*it)->assign(msg.client);
                }

                _connectionSocket[msg.client->getSocket()] = it->get();
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
        timeval t;
        FD_ZERO(&_rfds);
        FD_SET(_tcpConnection->getSocket(), &_rfds);
        for (auto &sock : _connections) {
            FD_SET(sock->getSocket(), &_rfds);
        }
        t.tv_sec = 1;
        t.tv_usec = 0;
        select((int)_connections.size() + 1, &_rfds, nullptr, nullptr, &t);
        for (auto &sock : _connections) {
            if (FD_ISSET(sock->getSocket(), &_rfds)) {
            }
        }
//        for (auto &item : _connectionSocket) {
//            if(item.second->hasReceived()) {
//                Packet pkg;
//
//                std::pair<SOCKET, BinaryData> receivedElem = item.second->read();
//                pkg.clientId = receivedElem.first;
//                pkg.data = receivedElem.second;
//
//                std::cout << pkg.data._data << std::endl;
//                events->send<Packet>(pkg);
//            }
//        }
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
