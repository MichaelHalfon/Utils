//
// Created by mboublil on 1/26/18.
//

#ifndef FUTILS_DATASTRUCTURES_HPP
#define FUTILS_DATASTRUCTURES_HPP

#include "Serializer/Serializable.hpp"

namespace mutils::net {

    struct MutilsPacket {
        std::size_t _size;
        std::string _data;
    };

    struct Header {
        std::uint16_t type;
        std::size_t size;
    };

    struct BinaryData {
        Header hdr;
        std::string _dataStr;
    };

    struct Packet {
        int clientId;
        BinaryData data;
    };

    struct Response {
        int clientId;
        BinaryData data;
    };

    struct DataInfos {
        int senderId{0};
        std::string ipAddress{};
        ssize_t size{0};
        std::string data{};
    };

    inline std::ostream &operator<<(std::ostream &os, Packet const &msg) {
        os.write(reinterpret_cast<const char *>(&msg.data.hdr.type), sizeof(std::uint16_t));
        os.write(reinterpret_cast<const char *>(&msg.data.hdr.size), sizeof(std::size_t));
        os.write(msg.data._dataStr.c_str(), msg.data.hdr.size);

        return os;

    }

}

#endif //FUTILS_DATASTRUCTURES_HPP
