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
        explicit AsyncSocket(std::shared_ptr<mutils::net::ITCPSocket> const &);
        ~AsyncSocket();

        // Adds a socket to vector of sockets
        void readContent();
        Header getHeader();


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
    private:
        std::unique_lock<std::mutex> _lk;
        std::shared_ptr<mutils::net::ITCPSocket> _sock;

        std::queue<std::pair<SOCKET, BinaryData>> _bufferSend;
        std::queue<std::pair<SOCKET, BinaryData>> _bufferRec;
        std::thread _clientThread;
        bool _stop { false };
    };

    extern std::mutex _mutexCv;
    extern std::vector<int> _fds;
    extern std::condition_variable _cv;
    extern bool dataProcessed;
}


#endif //NETWORK_ASYNCSOCKET_HPP
