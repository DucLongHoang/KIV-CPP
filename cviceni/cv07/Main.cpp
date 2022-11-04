#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <array>
#include <memory>
#include <algorithm>


template<typename T1>
T1 TemplateSum(T1 t1) {
    return t1;
}

template<typename T1, typename... T>
T1 TemplateSum(T1 t1, T... t) {
    return t1 + TemplateSum(t...);
}
//
//class Person {
//    private:
//        std::string mName;
//        int mYearBirth;
//
//    public:
//        Person(const std::string& name, int yearBirth) : mName(name), mYearBirth(yearBirth) {
//            std::cout << "creating " << name << " " << yearBirth << std::endl;
//        }
//
//        // copy
//        Person(const Person& l) = delete;
//        Person& operator=(const Person& l) = delete;
//
//        // move
//        Person(Person&& l)  noexcept : mName(l.mName), mYearBirth(l.mYearBirth) {
//            std::cout << "Move" << std::endl;
//        }
//        Person& operator=(Person&& l)  noexcept {
//            mName = l.mName; mYearBirth = l.mYearBirth;
//            std::cout << "Move" << std::endl;
//            return *this;
//        }
//};


template<typename T, typename... Targs>
std::unique_ptr<T> make(Targs&&... targs) {
    return std::unique_ptr<T>(new T(std::forward<Targs>(targs)...));
}

template<typename ... Targs>
int TemplateSum(Targs... targs) {
    (std::cout << ... << targs) << std::endl;
    return ((targs + 5) + ...);
}


/*
variadicke funkce

 1) any_of_contains - variadicka fce - prebira sadu kontejneru jako parametry (varia)
                    - zda nejaky z kontejneru obsahuje zadany prvek

                    any_of_contains(5, my_vector, my_list, my_list2);


 2) quoted_print - variadicka fce - prebira libovolne parametry, ktere jdou vypsat na proud (<<)
                 - vypise v uvozovkach, carkou oddelene parametry na std vystup

                 quoted_print("hello", "there", 42, 88.0)

                 "hello", "there", "42", "88.0"
 */

template<typename ... Targs>
void quoted_print(Targs ... targs) {
    (std::cout << ... << targs) << std::endl;
}



template<typename T1, typename T2>
bool any_of_contains(T1 t1, T2 t2) {
    bool found = false;

    for (auto i : t2) {
        if (t1 == i) {
            found = true;
            break;
        }
    }
    return found;
}

template<typename T1, typename T2, typename ... T>
bool any_of_contains(T1 t1, T2 t2, T ... t) {
    bool found = false;

    for (auto i : t2) {
        if (t1 == i) {
            found = true;
            break;
        }
    }

    return found || any_of_contains(t1, t...);
}

int main(int argc, char** argv) {

//    quoted_print("hello", "there", 42, 88.0);
    bool found = any_of_contains(5, std::vector<int>{1, 2, 3}, std::array<double, 3>{1.5, 2.2, 3.9}, std::set<int>{1, 2, 3, 4});
    std::cout << found << std::endl;
//    std::tuple<std::string , int, int> t1("aaa", 4, 4);
//
//    auto s = std::get<0>(t1);
//    auto i = std::get<1>(t1);
//
//    std::string s1;
//    int i1;
//    int j1;
//
//    std::tie(s1, i1, std::ignore) = t1;
//
//    std::cout << s << std::endl;
//    std::cout << i << std::endl;


//    auto p = TemplateSum(1, 2, 3, 4, 5);
//    std::cout << p << std::endl;
//    std::cout << TemplateSum(1, 2, 3, 4, 5, 6, 7, 8, 9) << std::endl;

//    auto p = make<Person>("Honza", 1999);
//    auto p2 = std::make_unique<Person>("Jan", 1988);

//    std::vector<Person> people;
//    people.push_back({"Antonin", 19});
//    people.emplace_back("Jiri", 1850);

    return EXIT_SUCCESS;
}