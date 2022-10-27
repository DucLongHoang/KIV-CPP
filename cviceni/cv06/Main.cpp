#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>

template<typename T>
class MyUniquePtr {
    protected:
        T* mData = nullptr;

    public:
        MyUniquePtr(T* p) : mData(p) {}

        // copy constructor - nope
        MyUniquePtr(const MyUniquePtr& other) = delete;
        // copy assignment - nope
        MyUniquePtr& operator=(const MyUniquePtr& other) = delete;

        // move constructor
        MyUniquePtr(MyUniquePtr&& other) noexcept : mData(other.mData) {
            other.mData = nullptr;
        }
        MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
            if (mData) delete mData;
            mData = other.mData;
            other.mData = nullptr;
            return *this;
        }

        // variadicke sablony + perfect forwarding
        static MyUniquePtr MakeUniquePtr(T val) {
            return MyUniquePtr(new T(val));
        }

        ~MyUniquePtr(){
            if (mData) delete mData;
        }
};


int main(int argc, char** argv) {

    MyUniquePtr<int> a = MyUniquePtr<int>::MakeUniquePtr(20);
    MyUniquePtr<int> b = std::move(a);
    MyUniquePtr<int> c(nullptr);


    c = std::move(b);

    MyUniquePtr<std::string> s = MyUniquePtr<std::string>::MakeUniquePtr("abc");

    std::vector<int> vect;


    return EXIT_SUCCESS;
}