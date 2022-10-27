//
// Created by Long on 29.09.2022.
//

#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <initializer_list>
#include <memory>

class Integer {
private:
    int mInt;

public:
    Integer(int a) : mInt{a} {
        std::cout << "constructing " << mInt << std::endl;
    }
    ~Integer() {
        std::cout << "destruction " << mInt << std::endl;
    }
    const int getInt() const {
        return mInt;
    }

};

int main(int argc, char** argv) {

    std::vector<int> a{1,2,3,489,456,1523,1,8564,53};
    a.reserve(128);

    std::cout << "size is " << a.size() << std::endl;
    std::cout << "capacity is " << a.capacity() << std::endl;
    std::cout << "first element @ is " << &a[0] << std::endl;

    a.push_back(6);
    a.push_back(6);
    a.push_back(6);

    std::cout << "size is " << a.size() << std::endl;
    std::cout << "capacity is " << a.capacity() << std::endl;
    std::cout << "first element @ is " << &a[0] << std::endl;

//    std::cout << "start" << std::endl;
//    std::shared_ptr<Integer> outerPtr;
//
//    {
//        std::shared_ptr<Integer> intPtr = std::make_shared<Integer>(5);
//        std::cout << "num is: " << intPtr->getInt() << std::endl;
//
//        outerPtr = intPtr;
//
//        std::cout << "num is: " << intPtr->getInt() << std::endl;
//    }
//
//
//    std::cout << "end" << std::endl;

//    std::map<std::string , double> myMap{
//            {"abc", 15},
//            {"def", 7.8},
//            {"stringy",5.998},
//    };
//
//    for (const auto& element : myMap) {
//        std::cout << element.first << " : " << element.second << std::endl;
//    }
//
//    std::string key = "a";
//    key += "bc";
//
//    std::cout << myMap[key] << std::endl;

//
////    std::cout << myMap[90] << std::endl;
//
//    auto itr = myMap.find(90);
//
//    if (itr != myMap.end()) {
//        std::cout << itr->second << std::endl;
//    }
//    else {
//        std::cout << "neni tu" << std::endl;
//    }
//
//    for (const auto& element : myMap) {
//        std::cout << element.first << " : " << element.second << std::endl;
//    }

//    std::array<int, 5> a{ 5, 2, 6, 9, 8, };
//    std::vector<int> b{ 2, 49, 51 , 88, 904 };
//
//    for (auto itr = b.begin(); itr != b.end(); ++itr) {
//        std::cout << *itr << std::endl;
//    }
//
//    std::cout << "-----" << std::endl;
//
//    for (auto itr = b.begin(); itr != b.end(); ++itr) {
//        std::cout << *itr << std::endl;
//        if ((*itr % 2) == 0) b.erase(itr);
//        else ++itr;
//    }
//
//    std::cout << "-----" << std::endl;
//
//    for (auto itr = b.begin(); itr != b.end(); ++itr) {
//        std::cout << *itr << std::endl;
//    }

    return EXIT_SUCCESS;
}