#include <vector>
#include "serializer/Serializable.hpp"

class C : public mutils::Serializable<C, mutils::out::JSONOutput>
{
public:
    C() : Serializable(std::make_pair("Header", &C::_type), std::make_pair("valueOfInt", &C::valueOfInt),
          std::make_pair("valueOfString", &C::valueOfString)), valueOfInt(10), valueOfString("mdr")
    {}

public:
    int             valueOfInt;
    std::string     valueOfString;

    std::string       _type = "Class C";
};

class B : public mutils::Serializable<B, mutils::out::JSONOutput>
{
public:
    B() : Serializable(std::make_pair("Header", &B::_type), std::make_pair("itsAnInt", &B::itsAnInt),
                       std::make_pair("testBool", &B::testBool), std::make_pair("itsAVectorOfClass", &B::itsAVector),
                       std::make_pair("itsAVectorOfInt", &B::itsAVector2)),
          itsAnInt(50), testBool(false), itsAVector2(3, 30)
    {
        C   obj1;
        C   obj2;
        C   obj3;

        obj1.valueOfInt = 5;
        obj3.valueOfInt = 15;
        this->itsAVector.push_back(obj1);
        this->itsAVector.push_back(obj2);
        this->itsAVector.push_back(obj3);
    }

public:
    int         itsAnInt;
    bool        testBool;
    std::vector<C>    itsAVector;
    std::vector<int>    itsAVector2;

    std::string       _type = "Class B";

};

class A : public mutils::Serializable<A, mutils::out::JSONOutput>
{

public:
    A() : Serializable(std::make_pair("Header", &A::_type), std::make_pair("x", &A::x),
                       std::make_pair("y", &A::y), std::make_pair("otherClass", &A::something)), x(5), y("lol") {}

public:
    int                 x;
    std::string         y;
    B                   something;

    std::string       _type = "ClassA";
};

int                     main()
{
    A                   obj;
    A                   obj1;
    std::stringstream   ss;

    obj.x = 15;
    ss << obj << std::endl;
    std::cout << ss.str() << std::endl;
    ss >> obj1;
    return 0;
}
