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
#include "ISocket.hpp"
#include "futils.hpp"
#include "types.hpp"
#include "../Serializer/Serializable.hpp"

namespace mutils::net {

    struct Header {
        std::uint16_t type;
        std::size_t size;
    };

    struct BinaryData {
        std::uint16_t type;
        std::string _data;
    };

    class AsyncSocket {
    public:
        AsyncSocket();
        ~AsyncSocket();

        //init of sockets + select();
        void analyzeSockets();

        // FD_ZERO and FD_SET
        void initSockets();

        // Adds a socket to vector of sockets
        bool assign(std::shared_ptr<ITCPSocket> const &sock);
        void readContent(ITCPSocket const *sock);
        Header getHeader(ITCPSocket const *sock);


        template<typename T>
        void write(SOCKET sock, Serializable<T> const &msg) {
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
        std::size_t getNbConnections() { return _sockets.size(); }

        static std::condition_variable _cv;
        static std::mutex _mutexCv;

    private:
        std::vector<std::shared_ptr<ITCPSocket>> _sockets;
        std::queue<std::pair<SOCKET, BinaryData>> _bufferSend;
        std::queue<std::pair<SOCKET, BinaryData>> _bufferRec;
        std::thread _clientThread;
        bool _stop { false };
        const int nbMaxConnections = 25;

        std::unique_lock<std::mutex> _lk;
    };
}


#endif //NETWORK_ASYNCSOCKET_HPP
