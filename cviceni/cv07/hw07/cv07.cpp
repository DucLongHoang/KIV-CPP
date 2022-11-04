#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include <array>

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
void quoted_print(Targs&& ... targs) {
    ( (std::cout << "\"" << targs << "\", ") , ... ) << std::endl;
}

template<typename T1, typename T2>
bool any_of_contains(T1 searched, T2 container) {
    bool found = false;

    for (auto i : container) {
        if (searched == i) {
            found = true;
            break;
        }
    }
    return found;
}

template<typename T1, typename T2, typename ... Targs>
bool any_of_contains(T1 searched, T2 container, Targs ... containers) {
    bool found = false;

    for (auto i : container) {
        if (searched == i) {
            found = true;
            break;
        }
    }

    return found || any_of_contains(searched, containers...);
}

int main(int argc, char** argv) {

    auto searched = 5;
    bool found = any_of_contains(searched, std::vector<int>{1, 2, 3}, std::array<double, 3>{1.5, 2.2, 3.9}, std::set<int>{1, 2, 3, 4});
    std::cout << "Was element " << searched << " found in the containers?: " << found << std::endl;

    quoted_print("hello", "there", 42, 88.0);


    return EXIT_SUCCESS;
}