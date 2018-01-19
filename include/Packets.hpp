//
// Created by mboublil on 1/12/18.
//

#ifndef NETWORK_PACKETS_HPP
#define NETWORK_PACKETS_HPP

#include "../mutils/lib/sources/Serializer/Serializable.hpp"

struct Packet : public mutils::Serializable<Packet> {
    Packet() : Serializable(&Packet::valueTest) {}

    std::string valueTest;
};

#endif //NETWORK_PACKETS_HPP
