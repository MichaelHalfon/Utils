//
// Created by mboublil on 1/9/18.
//

#include "Network.hpp"

namespace mutils {
    namespace net {
        Network::Network()
                : _tcpConnection(new TCPSocket()), _udpConnection(new UDPSocket()) {
        }
    }
}
