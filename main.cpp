#include "serializer/Serializable.hpp"

class A : public mutils::Serializable<A>
{

public:
    A() : Serializable(&A::x, &A::y), x(5), y(10) {}

private:
    int         x;
    int         y;

};

int	main()
{
    A   obj;

    std::cout << obj << std::endl;
    return 0;
}
