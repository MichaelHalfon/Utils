//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_UNIXSOCKET_HPP
# define MUTILS_UNIXSOCKET_HPP

# include "ISocket.hpp"

namespace  mutils {

    namespace net {
        class UnixSocket : ISocket {

            UnixSocket() = default;
            ~UnixSocket() = default;

            bool connect() override {}

            void disconnect() override {}

            void send() override {}

            void receive() override {}
        };
    }
}


#endif //MUTILS_UNIXSOCKET_HPP
