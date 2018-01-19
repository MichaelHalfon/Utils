//
// Created by mboublil on 1/11/18.
//

#ifndef NETWORK_LISTENER_HPP
#define NETWORK_LISTENER_HPP

#include <atomic>
#include "ecs.hpp"
#include "ISocket.hpp"
#include "Socket.hpp"

namespace mutils::net {

    struct newClientMessage {
        std::shared_ptr<ITCPSocket> client;
    };

    class Listener : public futils::ISystem {
    public:
        Listener();
        void run(float) override;
        void init();
        void waitForClients();
        void verifyNewClients();

    private:
        std::unique_ptr<ITCPSocket> _tcpConnection;
        std::queue< std::shared_ptr<ITCPSocket> > _clientQueue;
        std::thread _threadAction;
        int _phase {0};
        std::atomic<bool> _hasToEnd { false };
    };

}


#endif //NETWORK_LISTENER_HPP
