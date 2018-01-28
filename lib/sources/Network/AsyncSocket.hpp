//
// Created by mboublil on 1/12/18.
//

#ifndef NETWORK_ASYNCSOCKET_HPP
#define NETWORK_ASYNCSOCKET_HPP

#include <queue>
#include <functional>
#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "ISocket.hpp"
#include "futils.hpp"
#include "types.hpp"
#include "Serializer/Serializable.hpp"

namespace mutils::net {

    enum Actions {
        WRITE = 0,
        READ,
        NONE
    };

    class AsyncSocket {
    private:
        void readContent();
        Header getHeader();

        void writeContent();

    public:
        explicit AsyncSocket(ITCPSocket *);
        ~AsyncSocket();

        void write(MutilsPacket const &msg) {
            _bufferSend.push(msg);
        }

        void received(PacketReceived const &msg) {
            _bufferRec.push(msg);
        }

        PacketReceived read() {
            auto ret = _bufferRec.front();
            _bufferRec.pop();
            return ret;
        }

        MutilsPacket toSend() {
            auto ret = _bufferSend.front();
            _bufferRec.pop();
            return ret;
        }

        bool hasToSend() { return _bufferSend.size() > 0; }
        bool hasReceived() { return _bufferRec.size() > 0; }
    private:
        ITCPSocket *_sock;
        std::unique_lock<std::mutex> _lk;

        std::queue<MutilsPacket> _bufferSend;
        std::queue<PacketReceived> _bufferRec;
        std::thread _clientThread;
        bool _stop { false };
    };

    extern std::mutex _mutexCv;
    extern std::vector<int> _fds;
    extern std::condition_variable _cv;
    extern std::unordered_map<SOCKET, Actions> _action;
}


#endif //NETWORK_ASYNCSOCKET_HPP
