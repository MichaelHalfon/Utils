#ifdef _WIN32
#define _WINSOCKAPI_
#endif

#include "fender.hpp"
#include "Network/Network.hpp"

int main(int, char **av)
{
    auto pid = fork();

    fender::Fender engine(av[0]);

    if (engine.start() != 0)
        return 1;
    if (pid == 0) {
        engine.addSystem<mutils::net::Network>(true);
        engine.run();
    }
    else {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        engine.addSystem<mutils::net::Network>();
        engine.run();
    }
    return 0;
}
