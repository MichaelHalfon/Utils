//
// Created by mboublil on 1/9/18.
//

#include <memory>
#include <cstdint>
#include "Network.hpp"
#include "events.hpp"
#include "NetworkEvents.hpp"

namespace mutils::net {

    Network::Network(bool isServer)
            : _tcpConnection(new TCPSocket), _isServer(isServer) {
        if (isServer)
            name = "Server";
        else {
            name = "Client";
            _async = std::unique_ptr<AsyncSocket>(new AsyncSocket(_tcpConnection.get()));
        }
    }

    void Network::initializeFDs() {
        FD_ZERO(&_rfds);
        FD_SET(_tcpConnection->getSocket(), &_rfds);
        for (auto &sock : _connections) {
            FD_SET(sock->getSocket(), &_rfds);
        }
    }

    void Network::createNewClient() {
        auto conn = _tcpConnection->accept();
        std::cout << "new client!! ID: " << conn->getSocket() << std::endl;
        FD_SET(conn->getSocket(), &_rfds);
        _connectionSocket[conn->getSocket()] = new AsyncSocket(conn);
        _connections.push_back(std::unique_ptr<ITCPSocket>(conn));
//        events->send<newConnection>();
    }

    void Network::verifyFDs() {
        if (FD_ISSET(_tcpConnection->getSocket(), &_rfds))
            createNewClient();

        for (auto &sock : _connections) {
            if (FD_ISSET(sock->getSocket(), &_rfds)) {
                _fds.push_back(sock->getSocket());
                _action[sock->getSocket()] = Actions::READ;
                _cv.notify_all();
//                std::unique_lock<std::mutex> lk(_mutexCv);
//                _cv.wait(lk, [] {return dataProcessed;});
            }
        }
    }

    void Network::init(){
        ++_phase;

        addReaction<fender::events::Shutdown>([this](futils::IMediatorPacket &) {
            _shutdown = true;
            _listenerThread.join();
            entityManager->removeSystem(name);
        });

        if (_isServer) {
            _tcpConnection->bind(4242);
            _tcpConnection->listen();
            _listenerThread = std::thread([this]() {
                while (!_shutdown) {
                    timeval t;

                    t.tv_sec = 1;
                    t.tv_usec = 0;
                    initializeFDs();
                    select(getMax() + 1, &_rfds, nullptr, nullptr, &t);
                    verifyFDs();
                }
            });
        }
        else {
            addReaction<tryingToConnect>([this](futils::IMediatorPacket &pkg) {
                auto msg = futils::Mediator::rebuild<tryingToConnect>(pkg);

                try {
                    _tcpConnection->connect(msg.hostname, msg.port);
                }
                catch (std::exception const &e) {
                    error err;
                    err.message = e.what();

                    events->send<error>(err);
                }
            });
            _listenerThread = std::thread([this]() {
                while (!_shutdown) {
                    if (_connected) {
                        timeval t;

                        t.tv_sec = 1;
                        t.tv_usec = 0;
                        FD_ZERO(&_rfds);
                        FD_SET(_tcpConnection->getSocket(), &_rfds);
                        select(getMax() + 1, &_rfds, nullptr, nullptr, &t);
                        if (FD_ISSET(_tcpConnection->getSocket(), &_rfds)) {
                            _fds.push_back(_tcpConnection->getSocket());
                            _action[_tcpConnection->getSocket()] = Actions::READ;
                            _cv.notify_all();
                        }
                    }
                }
//                for (int i = 0; i < 3; i++) {
//                    std::this_thread::sleep_for(std::chrono::seconds(2));
//                    std::stringstream ss;
//                    Packet pkg;
//
//                    pkg.data.hdr.size = sizeof(test);
//                    pkg.data.hdr.type = 0x0001;
//                    test ts;
//
//                    ss << ts;
//                    pkg.data._dataStr = ss.str();
//                    _async->write(_tcpConnection->getSocket(), pkg);
//                    _fds.push_back(_tcpConnection->getSocket());
//                    _action[_tcpConnection->getSocket()] = Actions::WRITE;
//                    _cv.notify_all();
//                }
            });
        }
    }

    void Network::monitorConnections() {
        if (_isServer) {
            for (auto &item : _connectionSocket) {
                if (item.second->hasReceived()) {
                    Packet pkg;

                    std::pair<SOCKET, BinaryData> receivedElem = item.second->read();
                    pkg.clientId = receivedElem.first;
                    pkg.data = receivedElem.second;

//                    std::cout << "data read: " << pkg.data._dataStr.c_str() << " from client: " << item.first
//                              << std::endl;
                    events->send<Packet>(pkg);
                }
            }
        }
        else {
            if (_async->hasReceived()) {
                Packet pkg;

                std::pair<SOCKET, BinaryData> receivedElem = _async->read();
                pkg.clientId = receivedElem.first;
                pkg.data = receivedElem.second;

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
                    return monitorConnections();
            }
        }
    }
}
