//
// Created by mboublil on 12/2/17.
//

#include <criterion/criterion.h>
#include <unistd.h>
#include <memory>
#include <iostream>
#include "Network/UnixSockets.hpp"
#include "Tests.hpp"

Tests testNet;

void initClassAndSendSerToNetwork() {
    std::stringstream ss;

    testNet.firstElem.x = 140;
    testNet.firstElem.y = 150;
    testNet.secondElem.x = 160;
    testNet.secondElem.y = 170;
    int val1 = 110;
    int val2 = 120;
    int val3 = 130;
    std::vector<int> arrayOfInt = {60, 70, 80, 90, 100};
    std::vector<B> arrayOfSerClass = {testNet.firstElem, testNet.secondElem};
    std::vector<int *> arrayOfNakedPointer = {&val1, &val2, &val3};
    std::vector<std::shared_ptr<B>> arrayOfSharedSerClass = {std::make_shared<B>(testNet.firstElem), std::make_shared<B>(testNet.secondElem)};
    std::vector<B *> arrayOfNakerSerClassPointer = {&testNet.firstElem, &testNet.secondElem};
    std::unordered_map<int, std::shared_ptr<B>> mapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(testNet.firstElem)}, {val2, std::make_shared<B>(testNet.secondElem)}};
    std::map<int, std::shared_ptr<B>> orderedMapOfIntAndSharedPtrOnSerClass = {{val1, std::make_shared<B>(testNet.firstElem)}, {val2, std::make_shared<B>(testNet.secondElem)}};
    std::array<B *, 2> stdArrayOfSerClass = {&testNet.firstElem, &testNet.secondElem};
    std::list<B *> listOfSerClass {&testNet.firstElem, &testNet.secondElem};


    testNet.obj.w = std::make_unique<int>(5);
    testNet.obj.x = 10;
    testNet.obj.y = std::make_shared<int>(15);

    testNet.obj.z = &testNet.val;

    testNet.obj.str = "mdr";

    testNet.obj.elem.x = 25;
    testNet.obj.elem.y = 30;

    testNet.elemShared.x = 40;
    testNet.elemShared.y = 50;
    testNet.obj.ptrOnClass = std::make_shared<B>(testNet.elemShared);
    testNet.obj.arrayOfInt = arrayOfInt;
    testNet.obj.arrayOfSerClass = arrayOfSerClass;
    testNet.obj.arrayOfNakedPointer = arrayOfNakedPointer;
    testNet.obj.arrayOfSharedSerClass = arrayOfSharedSerClass;
    testNet.obj.arrayOfNakerSerClassPointer = arrayOfNakerSerClassPointer;
    testNet.obj.mapOfIntAndSharedPtrOnSerClass = mapOfIntAndSharedPtrOnSerClass;
    testNet.obj.orderedMapOfIntAndSharedPtrOnSerClass = orderedMapOfIntAndSharedPtrOnSerClass;
    testNet.obj.stdArrayOfSerClass = stdArrayOfSerClass;
    testNet.obj.listOfSerClass = listOfSerClass;

    pid_t pid = fork();
    if (pid == 0) {
        try {
            std::unique_ptr<mutils::net::ISocket> sock(new mutils::net::tcp::UnixSockets);
            std::stringstream ss;

            sock->bind(); // default value: 4242
            sock->listen();
            auto client = sock->accept();

            ss << testNet.obj;

            client->sendData(ss, sizeof(testNet.obj));

            exit(0);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    else {
        try {
            std::unique_ptr<mutils::net::ISocket> sock(new mutils::net::tcp::UnixSockets);
            sleep(2);

            sock->connect(); // default values: "localhost", 4242
            std::stringstream ss;

            sock->receiveData(ss, sizeof(testNet.obj));

            ss >> testNet.obj1;
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}

Test(assert, global, .init = initClassAndSendSerToNetwork) {

    //Simple scalar and standard string.
    cr_assert_eq(testNet.obj1.x, testNet.obj.x);
    cr_assert_eq(testNet.obj1.str, testNet.obj.str);
    cr_assert_eq(testNet.obj1.elem.x, testNet.obj.elem.x);
    cr_assert_eq(testNet.obj1.elem.y, testNet.obj.elem.y);

    // Different kind of pointers
    cr_assert_eq(*testNet.obj1.w, *testNet.obj.w);
    cr_assert_eq(*testNet.obj1.y, *testNet.obj.y);
    cr_assert_eq(*testNet.obj1.z, *testNet.obj.z);

    // Pointer on Class
    cr_assert_eq(testNet.obj1.ptrOnClass->x, testNet.obj.ptrOnClass->x);
    cr_assert_eq(testNet.obj1.ptrOnClass->y, testNet.obj.ptrOnClass->y);

    // Vectors of scalar, smart pointer, naked pointer
    cr_assert_eq(testNet.obj1.arrayOfInt, testNet.obj.arrayOfInt);
    bool check = std::equal(testNet.obj1.arrayOfSerClass.begin(), testNet.obj1.arrayOfSerClass.end(),
                            testNet.obj.arrayOfSerClass.begin(), Tests::compareClassSimple);
    cr_assert_eq(check, true);
    check = std::equal(testNet.obj1.arrayOfSharedSerClass.begin(), testNet.obj1.arrayOfSharedSerClass.end(),
                       testNet.obj.arrayOfSharedSerClass.begin(), Tests::compareClassSharedPointer);
    cr_assert_eq(check, true);
    check = std::equal(testNet.obj1.arrayOfNakerSerClassPointer.begin(), testNet.obj1.arrayOfNakerSerClassPointer.end(),
                       testNet.obj.arrayOfNakerSerClassPointer.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);

    // Map
    check = std::equal(testNet.obj1.orderedMapOfIntAndSharedPtrOnSerClass.begin(), testNet.obj1.orderedMapOfIntAndSharedPtrOnSerClass.end(),
                            testNet.obj.orderedMapOfIntAndSharedPtrOnSerClass.begin(), Tests::compareMaps);
    cr_assert_eq(check, true);


    // List
    check = std::equal(testNet.obj1.listOfSerClass.begin(), testNet.obj1.listOfSerClass.end(),
                       testNet.obj.listOfSerClass.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);

    // std::array
    check = std::equal(testNet.obj1.stdArrayOfSerClass.begin(), testNet.obj1.stdArrayOfSerClass.end(),
                       testNet.obj.stdArrayOfSerClass.begin(), Tests::compareClassPointer);
    cr_assert_eq(check, true);
}
