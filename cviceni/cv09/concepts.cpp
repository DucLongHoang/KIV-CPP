#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <type_traits>

template<typename T>
concept SignedInteger = std::is_signed_v<T> && std::is_integral_v<T>;

template<typename T>
concept FloatingValue = std::is_floating_point_v<T>;

template<typename T>
concept DoublePrecisionValue = FloatingValue<T> && sizeof(T) > 4;

template<typename T>
concept SelfMultipliable = requires(T t) {
    t * t;
};

template<typename T>
concept DynamicContainer = requires(T t, size_t size, std::ostream& os) {
    t.resize(size);
    { t.size() } -> std::convertible_to<size_t>;
    { os << t } -> std::convertible_to<std::ostream>;
};

//template<DynamicContainer T, typename U>
//concept DynamicDoubleEndedContainer = requires(T t, U u) {
//    t.push_front(u);
//};

//template<SignedInteger T>
//T multiply_by_2(T val) {
//    return val * 2;
//}

template<typename T>
requires SignedInteger<T> && SelfMultipliable<T>
T multiply_by_self(T val) {
    std::cout << "Multiplying integer: " << val << std::endl;
    return val * val;
}

template<FloatingValue T>
T multiply_by_2(T val) {
    return val * 2;
}

template<DoublePrecisionValue T>
T multiply_by_2(T val) {
    return val * 2;
}

template<DynamicContainer T>
void resize_container(T& cont) {
    cont.resize(10);
}

//template<DynamicDoubleEndedContainer<double> T>
//void resize_container(T& cont) {
//    cont.resize(10);
//}

class MyVec {
    public:
            void resize(size_t size);
            int size() {
                return 5;
            }
};

int main(int argc, char** argv) {
//    std::cout << multiply_by_2(5.5f) << std::endl;
//    std::cout << multiply_by_2(8.5) << std::endl;
//    std::cout << multiply_by_self(8) << std::endl;

    std::vector<int> a;
    MyVec b;

    resize_container(a);
    resize_container(b);
//    std::array<int, 5> b;

//    resize_container(a);
//    resize_container(b);

    return EXIT_SUCCESS;
}