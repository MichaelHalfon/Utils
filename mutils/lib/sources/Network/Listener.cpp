//
// Created by mboublil on 1/11/18.
//

#include <events.hpp>
#include "Listener.hpp"

namespace mutils::net {
    Listener::Listener()
            : _tcpConnection(new TCPSocket()) {
        name = "Listener";
    }

    void Listener::init() {
        _phase = 1;
        _tcpConnection->bind();
        _tcpConnection->listen();
        std::cout << "Accepting new clients." << std::endl;
        addReaction<fender::events::Shutdown>([this](futils::IMediatorPacket &) {
            _hasToEnd = true;
            _threadAction.join();
            entityManager->removeSystem(name);
        });

        _threadAction = std::thread([this]() {
            while (!_hasToEnd) {
                auto newClient = this->_tcpConnection->accept();
                if (newClient != nullptr)
                    this->_clientQueue.push(newClient);
            }
        });
    }

    void Listener::verifyNewClients() {
        if (_clientQueue.size() > 0) {
            newClientMessage msg;

            msg.client = _clientQueue.front();
            _clientQueue.pop();
            events->send<newClientMessage>(msg);
        }
    }

    void Listener::run(float) {
        switch (_phase) {
            case 0 :
                return init();
            default :
                return verifyNewClients();
        }
    }
}
