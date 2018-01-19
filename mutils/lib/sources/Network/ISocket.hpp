//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_ISOCKET_HPP
# define MUTILS_ISOCKET_HPP

# include <sstream>
# include <memory>

#ifdef __unix__
  using SOCKET = int;
#endif

namespace mutils::net {

    struct DataInfos {
        int senderId { 0 };
        std::string ipAddress { };
        ssize_t size { 0 };
        std::string data { };
    };

    class ISocket {
    public:
        virtual ~ISocket() = default;

        // Server Side
        virtual void bind(int port = 4242) = 0;

        // Client Side
        virtual void setServerInformations(const std::string &hostname = "localhost", int port = 4242) = 0;

        //Both sides
        virtual DataInfos receiveData(char *, size_t length) const = 0;
        virtual ssize_t sendData(const char *, size_t length) const = 0;

        SOCKET getSocket() const { return _socket; };

    public:
        SOCKET _socket;
    };

    class ITCPSocket : public ISocket {

    public:
        // Server Side
        virtual void listen() = 0;
        virtual std::shared_ptr<ITCPSocket> accept() = 0;

        // Client Side
        virtual void connect() = 0;

        // Both Sides
        virtual void disconnect() = 0;
    };

    class IUDPSocket : public ISocket {
        // Client Side
        virtual void setServerInformations(const std::string &hostname = "localhost", int port = 4343) override = 0;

    };
}

#endif //MUTILS_ISOCKET_HPP
