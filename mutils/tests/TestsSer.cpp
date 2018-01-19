//
// Created by mboublil on 11/27/17.
//

#include "Tests.hpp"

Tests testSer;

void initClassAndSerialization() {
    std::stringstream ss;

    testSer.firstElem.x = 140;
    testSer.firstElem.y = 150;
    testSer.secondElem.x = 160;
    testSer.secondElem.y = 170;
    int val1 = 110;
    int val2 = 120;
    int val3 = 130;
    std::vector<int> arrayOfInt = {60, 70, 80, 90, 100};
    std::vector<B> arrayOfSerClass = {testSer.firstElem, testSer.secondElem};
    std::vector<int *> arrayOfNakedPointer = {&val1, &val2, &val3};
    std::vector<std::shared_ptr<B>> arrayOfSharedSerClass = {std::make_shared<B>(testSer.firstElem), std::make_shared<B>(testSer.secondElem)};
    std::vector<B *> arrayOfNakerSerClassPointer = {&testSer.firstElem, &testSer.secondElem};
    std::unordered_map<int, std::shared_ptr<B>> mapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(testSer.firstElem)}, {val2, std::make_shared<B>(testSer.secondElem)}};
    std::map<int, std::shared_ptr<B>> orderedMapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(testSer.firstElem)}, {val2, std::make_shared<B>(testSer.secondElem)}};
    std::array<B *, 2> stdArrayOfSerClass = {&testSer.firstElem, &testSer.secondElem};
    std::list<B *> listOfSerClass {&testSer.firstElem, &testSer.secondElem};


    testSer.obj.w = std::make_unique<int>(5);
    testSer.obj.x = 10;
    testSer.obj.y = std::make_shared<int>(15);

    testSer.obj.z = &testSer.val;

    testSer.obj.str = "mdr";

    testSer.obj.elem.x = 25;
    testSer.obj.elem.y = 30;

    testSer.elemShared.x = 40;
    testSer.elemShared.y = 50;
    testSer.obj.ptrOnClass = std::make_shared<B>(testSer.elemShared);
    testSer.obj.arrayOfInt = arrayOfInt;
    testSer.obj.arrayOfSerClass = arrayOfSerClass;
    testSer.obj.arrayOfNakedPointer = arrayOfNakedPointer;
    testSer.obj.arrayOfSharedSerClass = arrayOfSharedSerClass;
    testSer.obj.arrayOfNakerSerClassPointer = arrayOfNakerSerClassPointer;
    testSer.obj.mapOfIntAndSharedPtrOnSerClass = mapOfIntAndSharedPtrOnSerClass;
    testSer.obj.orderedMapOfIntAndSharedPtrOnSerClass = orderedMapOfIntAndSharedPtrOnSerClass;
    testSer.obj.stdArrayOfSerClass = stdArrayOfSerClass;
    testSer.obj.listOfSerClass = listOfSerClass;

    ss << testSer.obj;
    ss >> testSer.obj1;
}

Test(asserts, scalarTypes, .init = initClassAndSerialization) {
    cr_assert_eq(testSer.obj1.x, testSer.obj.x);
    cr_assert_eq(testSer.obj1.str, testSer.obj.str);
}

Test(asserts, simpleClass, .init = initClassAndSerialization) {
    cr_assert_eq(testSer.obj1.elem.x, testSer.obj.elem.x);
    cr_assert_eq(testSer.obj1.elem.y, testSer.obj.elem.y);
}

Test(asserts, pointers, .init = initClassAndSerialization) {
    cr_assert_eq(*testSer.obj1.w, *testSer.obj.w);
    cr_assert_eq(*testSer.obj1.y, *testSer.obj.y);
    cr_assert_eq(*testSer.obj1.z, *testSer.obj.z);
    cr_assert_eq(testSer.obj1.ptrOnClass->x, testSer.obj.ptrOnClass->x);
    cr_assert_eq(testSer.obj1.ptrOnClass->y, testSer.obj.ptrOnClass->y);
}

Test(asserts, vectors, .init = initClassAndSerialization) {
    cr_assert_eq(testSer.obj1.arrayOfInt, testSer.obj.arrayOfInt);
    bool check = std::equal(testSer.obj1.arrayOfSerClass.begin(), testSer.obj1.arrayOfSerClass.end(),
                            testSer.obj.arrayOfSerClass.begin(), Tests::compareClassSimple);
    cr_assert_eq(check, true);
    check = std::equal(testSer.obj1.arrayOfSharedSerClass.begin(), testSer.obj1.arrayOfSharedSerClass.end(),
                       testSer.obj.arrayOfSharedSerClass.begin(), Tests::compareClassSharedPointer);
    cr_assert_eq(check, true);
    check = std::equal(testSer.obj1.arrayOfNakerSerClassPointer.begin(), testSer.obj1.arrayOfNakerSerClassPointer.end(),
                       testSer.obj.arrayOfNakerSerClassPointer.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);
}

Test(asserts, maps, .init = initClassAndSerialization) {
    bool check = std::equal(testSer.obj1.orderedMapOfIntAndSharedPtrOnSerClass.begin(),
                            testSer.obj1.orderedMapOfIntAndSharedPtrOnSerClass.end(),
                            testSer.obj.orderedMapOfIntAndSharedPtrOnSerClass.begin(), Tests::compareMaps);
    cr_assert_eq(check, true);
}

Test(asserts, listAndArray, .init = initClassAndSerialization) {
    bool check = std::equal(testSer.obj1.listOfSerClass.begin(), testSer.obj1.listOfSerClass.end(),
                            testSer.obj.listOfSerClass.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);
    check = std::equal(testSer.obj1.stdArrayOfSerClass.begin(), testSer.obj1.stdArrayOfSerClass.end(),
                       testSer.obj.stdArrayOfSerClass.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);
}
