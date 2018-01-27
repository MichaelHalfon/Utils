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

        void write(SOCKET sock, Packet const &msg) {
            std::stringstream ss;

            ss << msg;

            BinaryData data;

            data._dataStr = ss.str();
            data.hdr.type = msg.data.hdr.type;
            data.hdr.size = msg.data.hdr.size + sizeof(std::size_t) + sizeof(std::uint16_t);
            _bufferSend.push(std::make_pair(sock, data));
        }

        void received(SOCKET sock, BinaryData const &msg) {
            _bufferRec.push(std::make_pair(sock, msg));
        }

        std::pair<SOCKET, BinaryData> read() {
            auto ret = _bufferRec.front();
            _bufferRec.pop();
            return ret;
        }

        std::pair<SOCKET, BinaryData> toSend() {
            auto ret = _bufferSend.front();
            _bufferRec.pop();
            return ret;
        }

        bool hasToSend() { return _bufferSend.size() > 0; }
        bool hasReceived() { return _bufferRec.size() > 0; }
    private:
        ITCPSocket *_sock;
        std::unique_lock<std::mutex> _lk;

        std::queue<std::pair<SOCKET, BinaryData>> _bufferSend;
        std::queue<std::pair<SOCKET, BinaryData>> _bufferRec;
        std::thread _clientThread;
        bool _stop { false };
    };

    extern std::mutex _mutexCv;
    extern std::vector<int> _fds;
    extern std::condition_variable _cv;
    extern std::unordered_map<SOCKET, Actions> _action;
}


#endif //NETWORK_ASYNCSOCKET_HPP
