//
// Created by mboublil on 1/26/18.
//

#ifndef FUTILS_DATASTRUCTURES_HPP
#define FUTILS_DATASTRUCTURES_HPP

#include "Serializer/Serializable.hpp"

namespace mutils::net {

    struct Header {
        std::uint16_t type;
        std::size_t size;
    };

    struct BinaryData {
        Header hdr;
        std::string _data;
    };

    struct PacketRec {
        int clientId;
        BinaryData data;
    };

    struct PacketSend {
        Header hdr;
        ISerializable *ser;

    };

    struct DataInfos {
        int senderId{0};
        std::string ipAddress{};
        ssize_t size{0};
        std::string data{};
    };

    std::ostream &operator<<(std::ostream &, PacketSend const &);
}

#endif //FUTILS_DATASTRUCTURES_HPP
