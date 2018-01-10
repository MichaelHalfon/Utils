#include "fender.hpp"
#include "Network/Network.hpp"

int main(int, char **av)
{
    fender::Fender engine(av[0]);

    engine.addSystem<mutils::net::Network>();
    if (engine.start() != 0)
        return 1;
    engine.run();
    return 0;
}
