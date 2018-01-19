//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_INETWORK_HPP
#define MUTILS_INETWORK_HPP

#include <functional>

namespace mutils {
    namespace net {

        class INetwork {
            virtual void readHandler() = 0;
            virtual void writeHandler() = 0;
            virtual void setCommandFunction(uint16_t, std::function<void()>) = 0;
        };
    }
}


#endif //MUTILS_INETWORK_HPP
