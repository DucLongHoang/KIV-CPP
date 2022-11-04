#include <iostream>
#include <string>
#include <vector>

// implementovat genericky vektor
// ktery lze parametrizovat typem i hodnotou
// typ = datovy typ prvku vektoru
// hodnota = dimenze vektoru

// Vector<float, 2> --> [1.0f, 2.0f]
// Vector<int, 3> --> [3, 4, 5]

// jde udelat
// operator+ - secist vektory
// operator* - skalarni soucin

// umoznit vykonstruovani z std::array a z std::vector
// konstrukce z std::array = static_assert,
//    overuje ze velikost std::array souhlasi s dimenzi vektoru
//    std::is_same_v

// std::array<int,3> arr{3, 4}
// Vector<int, 3> a(arr); - not ok !!!!!!

// volitelne
// static_assert(arr.size() == DIM, "chyba!");
// static_assert(std::is_same_v<T(a), T(b)>, "typy nejsou stejne");
// static_assert(std::is_same_v<T, std::array<int, 4>::value_type>, "typy nejsou stejne");

template<typename T, typename std::enable_if<std::is_integral_v<T>>* = nullptr>
void print_num(const T& num) {
    std::cout << "num is: " << num << std::endl;
}

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

    print_num(5);
    print_num("dsd");

//    MyUniquePtr<int> a = MyUniquePtr<int>::MakeUniquePtr(20);
//    MyUniquePtr<int> b = std::move(a);
//    MyUniquePtr<int> c(nullptr);
//
//
//    c = std::move(b);
//
//    MyUniquePtr<std::string> s = MyUniquePtr<std::string>::MakeUniquePtr("abc");
//
//    std::vector<int> vect;


    return EXIT_SUCCESS;
}