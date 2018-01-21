//
// Created by mboublil on 12/2/17.
//

#ifndef MUTILS_TESTS_HPP
# define MUTILS_TESTS_HPP

# include <criterion/criterion.h>
# include <algorithm>
# include <sstream>
# include "Serializer/Serializable.hpp"

class B : public mutils::Serializable<B>
{

public:
    B() : Serializable(&B::x, &B::y), x(0), y(0) {}


public:
    int x;
    int y;
};

class A : public mutils::Serializable<A>
{

public:
    A() : Serializable(&A::w, &A::x, &A::y, &A::z, &A::str, &A::elem, &A::ptrOnClass, &A::arrayOfInt,
                       &A::arrayOfSerClass, &A::arrayOfNakedPointer, &A::arrayOfSharedSerClass,
                       &A::arrayOfNakerSerClassPointer,
                       &A::mapOfIntAndSharedPtrOnSerClass,
                       &A::orderedMapOfIntAndSharedPtrOnSerClass,
                       &A::stdArrayOfSerClass,
                       &A::listOfSerClass), x(0), z(0), str("") {}
    ~A() {}

public:
    std::unique_ptr<int> w;
    int x;
    std::shared_ptr<int> y;
    int *z;
    std::string str;

    B elem;
    std::shared_ptr<B> ptrOnClass;
    std::vector<int> arrayOfInt;
    std::vector<B> arrayOfSerClass;
    std::vector<int *> arrayOfNakedPointer;
    std::vector<std::shared_ptr<B>> arrayOfSharedSerClass;
    std::vector<B *> arrayOfNakerSerClassPointer;
    std::unordered_map<int, std::shared_ptr<B>> mapOfIntAndSharedPtrOnSerClass;
    std::map<int, std::shared_ptr<B>> orderedMapOfIntAndSharedPtrOnSerClass;
    std::array<B *, 2> stdArrayOfSerClass;
    std::list<B *> listOfSerClass;
};

struct Tests {
    A obj;
    A obj1;
    B elemShared;
    B firstElem;
    B secondElem;
    int val = 20;

    static bool compareClassSimple(B elem1, B elem2) {
        return (elem1.x == elem2.x && elem1.y == elem2.y);
    }

    static bool compareClassPointer(B* elem1, B* elem2) {
        return (elem1->x == elem2->x && elem1->y == elem2->y);
    }

    static bool compareClassSharedPointer(std::shared_ptr<B> elem1, std::shared_ptr<B> elem2) {
        return (elem1->x == elem2->x && elem1->y == elem2->y);
    }

    static bool compareMaps(std::pair<int, std::shared_ptr<B>> elem1, std::pair<int, std::shared_ptr<B>> elem2) {
        return (elem1.first == elem2.first && elem1.second->x == elem2.second->x && elem1.second->y == elem2.second->y);
    }
};

#endif //MUTILS_TESTS_HPP
