//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_NETWORK_HPP
#define MUTILS_NETWORK_HPP

#include <memory>
#include "Socket.hpp"
#include "ecs.hpp"

namespace mutils {
    namespace net {
        class Network : public futils::ISystem {
        public:
            Network();

            void run(float) {}

        private:
            std::unique_ptr<ITCPSocket> _tcpConnection;
            std::unique_ptr<IUDPSocket> _udpConnection;
        };

    }
}



#endif //MUTILS_NETWORK_HPP
