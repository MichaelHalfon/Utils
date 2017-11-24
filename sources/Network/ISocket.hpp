//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_ISOCKET_HPP
#define MUTILS_ISOCKET_HPP

namespace mutils {
    namespace net {

        enum connType {
            TCP = 0,
            UDP
        };

        class ISocket {
            virtual ~ISocket() = default;

        public:
            virtual bool connect() = 0;

            virtual void disconnect() = 0;

            virtual void send() = 0;

            virtual void receive() = 0;
        };
    }
}

#endif //MUTILS_ISOCKET_HPP
