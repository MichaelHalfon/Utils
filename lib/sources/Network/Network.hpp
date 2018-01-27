//
// Created by mboublil on 1/9/18.
//

#ifndef MUTILS_NETWORK_HPP
#define MUTILS_NETWORK_HPP

#include <memory>
#include <vector>
#include "Socket.hpp"
#include "ecs.hpp"
#include "AsyncSocket.hpp"
#include "NetworkEvents.hpp"

namespace mutils::net {

    class Network : public futils::ISystem {
    public:
        explicit Network(bool isServer = false);
        void run(float) override;

    private:
        void init();
        void monitorConnections();
        void initializeFDs();
        void verifyFDs();
        void createNewClient();
        SOCKET getMax() {
            SOCKET max = _tcpConnection->getSocket();
            for (auto &sock : _connections) {
                if (max < sock->getSocket())
                    max = sock->getSocket();
            }
            return max;
        }

        template <typename T>
        static std::string serializeClass(T pkg) {
            static_assert(std::is_base_of<Serializable<T>, T>(), "Wrong class sent to function serializeClass()");

            std::stringstream ss;

            ss << pkg;

	    return ss.str();
        }

    private:
        // Both
        std::unique_ptr<ITCPSocket> _tcpConnection;
        std::vector<std::unique_ptr<IUDPSocket>> _udpConnections;
        bool _isServer;
        int _phase{0};
        float _sumTime { 0 };
        bool _shutdown { false };
        bool _connected { false };
        fd_set _rfds;

        // Client
        std::unique_ptr<AsyncSocket> _async;

        // Server
        std::vector<std::unique_ptr<ITCPSocket>> _connections;
        std::unordered_map<int, AsyncSocket *> _connectionSocket;
        std::thread _listenerThread;
    };
}

#endif //MUTILS_NETWORK_HPP
