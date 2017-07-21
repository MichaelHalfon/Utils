#include "serializer/Serializable.hpp"

class A : public mutils::Serializable<A, mutils::out::DebugOutput>
{

public:
    A() : Serializable(std::make_pair("Header", &A::_type), std::make_pair("x", &A::x), std::make_pair("y", &A::y)), x(5), y(10) {}

private:
    int         x;
    int         y;

    const std::string       _type = "Class A";
};

int                     main()
{
    A                   obj;
    std::stringstream   ss;

    ss << obj << std::endl;
    return 0;
}
