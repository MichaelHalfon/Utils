#include "serializer/Serializable.hpp"

class A : public mutils::Serializable<A, mutils::out::JSONOutput>
{

public:
    A() : Serializable(std::make_pair("Header", &A::_type), std::make_pair("x", &A::x), std::make_pair("y", &A::y)), x(5), y("lol") {}

public:
    int         x;
    std::string         y;

    std::string       _type = "Class A";
};

int                     main()
{
    A                   obj;
    A                   obj1;
    std::stringstream   ss;

    obj.x = 15;
    ss << obj << std::endl;
    std::cout << ss.str() << std::endl;
//    ss << obj << std::endl;
//    ss >> obj1;
//    std::cout << obj1 << std::endl;
    return 0;
}
