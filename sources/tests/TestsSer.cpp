//
// Created by mboublil on 11/27/17.
//

#include <criterion/criterion.h>
#include <algorithm>
#include <sstream>
#include "Serializable.hpp"

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

static A obj;
static A obj1;
static B elemShared;
static B firstElem;
static B secondElem;
static int val = 20;

bool compareClassSimple(B elem1, B elem2) {
    return (elem1.x == elem2.x && elem1.y == elem2.y);
}

bool compareClassPointer(B* elem1, B* elem2) {
    return (elem1->x == elem2->x && elem1->y == elem2->y);
}

bool compareClassSharedPointer(std::shared_ptr<B> elem1, std::shared_ptr<B> elem2) {
    return (elem1->x == elem2->x && elem1->y == elem2->y);
}

bool compareMaps(std::pair<int, std::shared_ptr<B>> elem1, std::pair<int, std::shared_ptr<B>> elem2) {
    return (elem1.first == elem2.first && elem1.second->x == elem2.second->x && elem1.second->y == elem2.second->y);
}

void initClassAndSerialization() {
    std::stringstream ss;

    firstElem.x = 140;
    firstElem.y = 150;
    secondElem.x = 160;
    secondElem.y = 170;
    int val1 = 110;
    int val2 = 120;
    int val3 = 130;
    std::vector<int> arrayOfInt = {60, 70, 80, 90, 100};
    std::vector<B> arrayOfSerClass = {firstElem, secondElem};
    std::vector<int *> arrayOfNakedPointer = {&val1, &val2, &val3};
    std::vector<std::shared_ptr<B>> arrayOfSharedSerClass = {std::make_shared<B>(firstElem), std::make_shared<B>(secondElem)};
    std::vector<B *> arrayOfNakerSerClassPointer = {&firstElem, &secondElem};
    std::unordered_map<int, std::shared_ptr<B>> mapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(firstElem)}, {val2, std::make_shared<B>(secondElem)}};
    std::map<int, std::shared_ptr<B>> orderedMapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(firstElem)}, {val2, std::make_shared<B>(secondElem)}};
    std::array<B *, 2> stdArrayOfSerClass = {&firstElem, &secondElem};
    std::list<B *> listOfSerClass {&firstElem, &secondElem};


    obj.w = std::make_unique<int>(5);
    obj.x = 10;
    obj.y = std::make_shared<int>(15);

    obj.z = &val;

    obj.str = "mdr";

    obj.elem.x = 25;
    obj.elem.y = 30;

    elemShared.x = 40;
    elemShared.y = 50;
    obj.ptrOnClass = std::make_shared<B>(elemShared);
    obj.arrayOfInt = arrayOfInt;
    obj.arrayOfSerClass = arrayOfSerClass;
    obj.arrayOfNakedPointer = arrayOfNakedPointer;
    obj.arrayOfSharedSerClass = arrayOfSharedSerClass;
    obj.arrayOfNakerSerClassPointer = arrayOfNakerSerClassPointer;
    obj.mapOfIntAndSharedPtrOnSerClass = mapOfIntAndSharedPtrOnSerClass;
    obj.orderedMapOfIntAndSharedPtrOnSerClass = orderedMapOfIntAndSharedPtrOnSerClass;
    obj.stdArrayOfSerClass = stdArrayOfSerClass;
    obj.listOfSerClass = listOfSerClass;

    ss << obj;
    ss >> obj1;
}

Test(asserts, scalarTypes, .init = initClassAndSerialization) {

    cr_assert_eq(obj1.x, obj.x);
    cr_assert_eq(obj1.str, obj.str);
}

Test(asserts, simpleClass, .init = initClassAndSerialization) {
    cr_assert_eq(obj1.elem.x, obj.elem.x);
    cr_assert_eq(obj1.elem.y, obj.elem.y);
}

Test(asserts, pointers, .init = initClassAndSerialization) {
    cr_assert_eq(*obj1.w, *obj.w);
    cr_assert_eq(*obj1.y, *obj.y);
    cr_assert_eq(*obj1.z, *obj.z);
    cr_assert_eq(obj1.ptrOnClass->x, obj.ptrOnClass->x);
    cr_assert_eq(obj1.ptrOnClass->y, obj.ptrOnClass->y);
}

Test(asserts, vectors, .init = initClassAndSerialization) {
    cr_assert_eq(obj1.arrayOfInt, obj.arrayOfInt);
    bool check = std::equal(obj1.arrayOfSerClass.begin(), obj1.arrayOfSerClass.end(), obj.arrayOfSerClass.begin(), compareClassSimple);
    cr_assert_eq(check, true);
    check = std::equal(obj1.arrayOfSharedSerClass.begin(), obj1.arrayOfSharedSerClass.end(), obj.arrayOfSharedSerClass.begin(), compareClassSharedPointer);
    cr_assert_eq(check, true);
    check = std::equal(obj1.arrayOfNakerSerClassPointer.begin(), obj1.arrayOfNakerSerClassPointer.end(), obj.arrayOfNakerSerClassPointer.begin(), compareClassPointer);
    cr_assert_eq(check, true);
}

Test(asserts, maps, .init = initClassAndSerialization) {
    bool check = std::equal(obj1.orderedMapOfIntAndSharedPtrOnSerClass.begin(), obj1.orderedMapOfIntAndSharedPtrOnSerClass.end(),
                            obj.orderedMapOfIntAndSharedPtrOnSerClass.begin(), compareMaps);
    cr_assert_eq(check, true);
}

Test(asserts, listAndArray, .init = initClassAndSerialization) {
    bool check = std::equal(obj1.listOfSerClass.begin(), obj1.listOfSerClass.end(), obj.listOfSerClass.begin(), compareClassPointer);
    cr_assert_eq(check, true);
    check = std::equal(obj1.stdArrayOfSerClass.begin(), obj1.stdArrayOfSerClass.end(), obj.stdArrayOfSerClass.begin(), compareClassPointer);
    cr_assert_eq(check, true);
}
