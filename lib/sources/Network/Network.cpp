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
            _async = new AsyncSocket(_tcpConnection.get());
        }
    }

    void Network::waitEndProcess() {
        {
            std::unique_lock<std::mutex> lk(_mutexCv);
            _cv.wait(lk, [this] { return _process._isFinished; });
            _process._isFinished = false;
        }
        if (_process.status != Status::ALL_GOOD) {
            disconnection d;

            d._id = _process.id;
            std::cerr << "Problem during the processing of socket: " << _process.id << ". Closing connection." << std::endl;
            events->send<disconnection>(d);
        }
        _process.status = Status::ALL_GOOD;
    }

    void Network::initializeFDs() {
        FD_ZERO(&_rfds);
        FD_SET(_tcpConnection->getSocket(), &_rfds);
        for (auto &sock : _connections) {
            FD_SET(sock->getSocket(), &_rfds);
        }
    }

    void Network::sendHandShake() {
        handshake h;
        std::stringstream ss;
        MutilsPacket p;
        std::size_t size = sizeof(std::uint32_t);

        ss.write(reinterpret_cast<const char *>(&h.type), sizeof(std::uint16_t));
        ss.write(reinterpret_cast<const char *>(&size), sizeof(std::size_t));
        ss.write(reinterpret_cast<const char *>(&h.value), sizeof(std::uint32_t));
        p._data = ss.str();
        if (_isServer)
            p._id = _connections.back()->getSocket();
        else
            p._id = _tcpConnection->getSocket();
        p._size = sizeof(std::uint16_t) + sizeof(std::size_t) + sizeof(std::uint32_t);
        std::cout << "[ " << name << " ]: " << "Sending handshake..." << std::endl;
        events->send<MutilsPacket>(p);
   }

    void Network::newClientStartProcess() {
        auto conn = _tcpConnection->accept();
        std::cout << "new client!! ID: " << conn->getSocket() << std::endl;
        FD_SET(conn->getSocket(), &_rfds);
        _connectionSocket[conn->getSocket()] = std::unique_ptr<AsyncSocket>(new AsyncSocket(conn));
        _connections.push_back(std::unique_ptr<ITCPSocket>(conn));
    }

    void Network::verifyFDs() {
        if (FD_ISSET(_tcpConnection->getSocket(), &_rfds))
            newClientStartProcess();

        else {
            std::size_t size = _connections.size();
            for (auto &sock : _connections) {
                if (FD_ISSET(sock->getSocket(), &_rfds)) {
                    _fds.push_back(sock->getSocket());
                    _action[sock->getSocket()] = Actions::READ;
                    _notified = true;
                    _cv.notify_all();
                    waitEndProcess();
                    if (size != _connections.size())
                        break ;
                }
            }
        }
    }

    void Network::init(){
        ++_phase;

        addReaction<fender::events::Shutdown>([this](futils::IMediatorPacket &) {
            _shutdown = true;
            _connectionThread.join();
            if (!_isServer) {
                delete _async;
            }
            entityManager->removeSystem(name);
        });

        //TODO: A MODIFIER ABSOLUMENT.
        addReaction<disconnection>([this](futils::IMediatorPacket &pkg) {
            auto p = futils::Mediator::rebuild<disconnection>(pkg);

            if (_isServer) {
                auto it = std::find_if(_connections.begin(), _connections.end(), [&p](std::unique_ptr<ITCPSocket> &conn) {
                    return conn->getSocket() == p._id;
                });
                if (it != _connections.end()) {
                    _connectionSocket.erase(p._id);
                    std::cout << "erasing connection: " << (*it)->getSocket() << std::endl;
                    _connections.erase(it);
                    std::cout << "Connection erased !" << std::endl;
                }
            }
            else {
                delete _async;
                _async = nullptr;
                _tcpConnection->disconnect();
                _connected = false;
            }
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
            _notified = true;
            _cv.notify_all();
            waitEndProcess();
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
                    sendHandShake();
                    _connected = true;
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
                            _notified = true;
                            _cv.notify_all();
                            waitEndProcess();
                        }
                    }
                }
            });
        }
    }

    void Network::monitorConnections() {
        if (_isServer) {
            for (auto &item : _connectionSocket) {
                if (item.second->hasReceived()) {
                    PacketReceived pkg = item.second->read();

                    if (pkg._type == 0x0000) {
                        std::uint32_t const *value = reinterpret_cast<std::uint32_t const *>(pkg._data.c_str());
                        if (*value == handshakeValue) {
                            std::cout << "handshake confirmed !" << std::endl;
                            newConnection msg;
                            msg._id = pkg._id;
                            events->send<newConnection>(msg);
                        }
                    }
                    else {
                        std::cout << "data read: " << pkg._data << " from client: " << pkg._id << std::endl;
                        events->send<PacketReceived>(pkg);
                    }
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
