//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_ISOCKET_HPP
# define MUTILS_ISOCKET_HPP

# include <sstream>

namespace mutils {
    namespace net {

        class ISocket {
        public:
            virtual ~ISocket() = default;

            // Server Side
            virtual void bind(int port = 4242) = 0;
            virtual void listen() = 0;
            virtual std::unique_ptr<ISocket> accept() = 0;

            // Client Side
            virtual void connect(const std::string &hostname = "localhost", int port = 4242) = 0;

            // Both sides
            virtual ssize_t sendData(const std::stringstream &is, size_t length) = 0;
            virtual ssize_t receiveData(std::stringstream &os, size_t length) = 0;
        };
    }
}

#endif //MUTILS_ISOCKET_HPP
