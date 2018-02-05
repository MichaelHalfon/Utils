//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_ISOCKET_HPP
# define MUTILS_ISOCKET_HPP

# include <sstream>
# include <memory>
# include "DataStructures.hpp"

#ifdef __unix__
  using SOCKET = int;

#elif _WIN32

  #include <WinSock2.h>

  using ssize_t = intptr_t;

#endif


namespace mutils::net {

    namespace msg {
        const std::string errorCreationSocket = "Error during the creation of the socket.";
        const std::string errorBindSocket = "Error during the binding of the socket.";
        const std::string errorListenSocket = "Error during the listening on the socket.";
        const std::string errorAcceptClient = "Error during the accepting of a new client.";
        const std::string errorUnknownHostname = "Error: unknown hostname.";
        const std::string errorConnectionServer = "Error during the connection on the server.";
        const std::string errorCloseSocket = "Error during the closing of the socket.";
    }

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
        SOCKET _socket { -1 };
    };

    class ITCPSocket : public ISocket {

    public:
        // Server Side
        virtual void listen() = 0;
        virtual ITCPSocket *accept() = 0;

        // Client Side
        virtual void connect() = 0;
        virtual void connect(const std::string &hostname, int port = 4242) = 0;

        // Both Sides
        virtual void disconnect() = 0;
    };

    class IUDPSocket : public ISocket {
        // Client Side
        virtual void setServerInformations(const std::string &hostname = "localhost", int port = 4343) override = 0;

    };
}

#endif //MUTILS_ISOCKET_HPP
