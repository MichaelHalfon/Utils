//
// Created by mboublil on 1/9/18.
//

#include <memory>
#include <cstdint>
#include "Network.hpp"
#include "events.hpp"

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
        _connectionSocket[conn->getSocket()] = std::unique_ptr<AsyncSocket>(new AsyncSocket(conn));
        _connections.push_back(std::unique_ptr<ITCPSocket>(conn));

        newConnection pkg;
        pkg._id = conn->getSocket();
        events->send<newConnection>(pkg);
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
            _connectionThread.join();
            entityManager->removeSystem(name);
        });

        addReaction<disconnection>([this](futils::IMediatorPacket &pkg) {
            auto p = futils::Mediator::rebuild<disconnection>(pkg);
            if (_isServer) {
                _connectionSocket.erase(p._id);
                auto it = std::find_if(_connections.begin(), _connections.end(), [&p](std::unique_ptr<ITCPSocket> &conn) {
                    return conn->getSocket() == p._id;
                });
                if (it != _connections.end())
                    _connections.erase(it);
            }
            else
                _tcpConnection->disconnect();
        });

        addReaction<MutilsPacket>([this](futils::IMediatorPacket &pkg) {
            auto p = futils::Mediator::rebuild<MutilsPacket>(pkg);

            if (_isServer) {
                _connectionSocket[p._id]->write(p);
            }
            else
                _async->write(p);
            _fds.push_back(p._id);
            _action[p._id] = Actions::WRITE;
            _cv.notify_all();
        });

        if (_isServer) {
            _tcpConnection->bind(4242);
            _tcpConnection->listen();
            _connectionThread = std::thread([this]() {
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
            _connectionThread = std::thread([this]() {
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
                    PacketReceived pkg = item.second->read();

                    std::cout << "data read: " << pkg._data << " from client: " << pkg._id << std::endl;
                    events->send<PacketReceived>(pkg);
                }
            }
        }
        else {
            if (_async->hasReceived()) {
                PacketReceived pkg = _async->read();

                pkg._id = _tcpConnection->getSocket();

                events->send<PacketReceived>(pkg);
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
