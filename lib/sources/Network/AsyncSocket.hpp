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
        READ
    };

    class AsyncSocket {
    private:
        void readContent();
        Header getHeader();

        void writeContent();

    public:
        explicit AsyncSocket(ITCPSocket *);
        ~AsyncSocket();


        void write(SOCKET sock, PacketSend const &msg) {
            std::stringstream ss;

            ss << msg;

            BinaryData data;

            data._data = ss.str();
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
        std::unique_lock<std::mutex> _lk;
        ITCPSocket *_sock;

        std::queue<std::pair<SOCKET, BinaryData>> _bufferSend;
        std::queue<std::pair<SOCKET, BinaryData>> _bufferRec;
        std::thread _clientThread;
        bool _stop { false };
    };

    inline std::ostream &operator<<(std::ostream &os, PacketSend const &msg) {
        os.write(reinterpret_cast<const char *>(&msg.hdr.type), sizeof(std::uint16_t));
        os.write(reinterpret_cast<const char *>(&msg.hdr.size), sizeof(std::size_t));

        os << msg.ser;
        return os;
    }

    extern std::mutex _mutexCv;
    extern std::vector<int> _fds;
    extern std::condition_variable _cv;
    extern std::unordered_map<SOCKET, Actions> _action;
}


#endif //NETWORK_ASYNCSOCKET_HPP
