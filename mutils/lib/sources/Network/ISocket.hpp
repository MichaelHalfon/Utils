//
// Created by mboublil on 11/19/17.
//

#ifndef MUTILS_ISOCKET_HPP
# define MUTILS_ISOCKET_HPP

# include <sstream>

namespace mutils {
    namespace net {

        //TODO: structure contenant les informations du client et la data reçue. Valeur de retour de receiveData
        struct clientInfo {
            int id { 0 };
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
            virtual clientInfo receiveData(std::stringstream &os, size_t length) = 0;
            virtual ssize_t sendData(const std::stringstream &is, size_t length) = 0;

        };

        class ITCPSocket : public ISocket {

        public:
            // Server Side
            virtual void listen() = 0;
            virtual std::unique_ptr<ITCPSocket> accept() = 0;
        };

        class IUDPSocket : public ISocket {
            // Client Side
            virtual void setServerInformations(const std::string &hostname = "localhost", int port = 4343) override = 0;

        };
    }
}

#endif //MUTILS_ISOCKET_HPP
