#ifdef _WIN32
#define _WINSOCKAPI_
#endif

#include "fender.hpp"
#include "Network/Network.hpp"

int main(int, char **av)
{
    fender::Fender engine(av[0]);

    if (engine.start() != 0)
        return 1;
    engine.addSystem<mutils::net::Network>(true);
    engine.addSystem<mutils::net::Network>();
    engine.run();
    return 0;
}
