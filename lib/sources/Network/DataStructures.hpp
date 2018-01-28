//
// Created by mboublil on 1/26/18.
//

#ifndef FUTILS_DATASTRUCTURES_HPP
#define FUTILS_DATASTRUCTURES_HPP

#include "Serializer/Serializable.hpp"

namespace mutils::net {

    enum Status {
        ALL_GOOD = 0,
        SOCKET_CLOSE = 1,
        OTHER_ERROR = 2
    };

    struct MutilsPacket {
        int _id;
        std::size_t _size;
        std::string _data;
    };

    struct PacketReceived {
        int _id;
        std::uint16_t _type;
        std::size_t _size;
        std::string _data;
    };


    struct Header {
        std::uint16_t type;
        std::size_t size;
    };

    struct DataInfos {
        int senderId{0};
        std::string ipAddress{};
        ssize_t size{0};
        std::string data{};
    };

    struct dataProcessed {
        bool _isFinished { false };
        int id { -1 };
        Status status { Status::ALL_GOOD };
    };
}

#endif //FUTILS_DATASTRUCTURES_HPP
