//
// Created by mboublil on 1/26/18.
//

#ifndef FUTILS_DATASTRUCTURES_HPP
#define FUTILS_DATASTRUCTURES_HPP

#include "Serializer/Serializable.hpp"

namespace mutils::net {

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

}

#endif //FUTILS_DATASTRUCTURES_HPP
