#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <map>

/*
 * Ukol
 * napsat metodu co rekurzivne projde adresarovou strukturu = i podslozky (is_directory)
 *  - lze pouzit rekurzi
 *      walk(const std::filesystem::path& cesta)
 *  - rozpoznat soubory: textove .txt (regex, path::extension, string_view),
 *                       obrazky .png, bmp, jpg, jpeg
 *  - vysledek vratit jako jako vektor std::tuple - jmeno souboru, typ souboru (enum class), velikost
 *  - vysledek vypsat, structured binding
 */

std::map<int, std::string> cisla = {
        {1, "jedna"},
        {2, "dva"},
        {6, "sest"},
};

std::map<int, std::variant<double, std::string>> cisla2 = {
        {1, "jedna"},
        {2, 2.0},
        {6, "sest"},
};

std::map<int, std::any> cisla3 = {
        {1, "jedna"},
        {2, 2.0},
        {6, "sest"},
};

std::optional<std::string> najdi_cislo(int a) {
    if (cisla.contains(a))
        return cisla[a];
    return std::nullopt;
}

int main(int argc, char** argv) {

//    auto nalezene = najdi_cislo(2);
//    std::cout << "Nalezene cislo je: " << (nalezene.has_value() ? nalezene.value() : "Nenalezeno") << std::endl;
//
//    nalezene = najdi_cislo(3);
//    std::cout << "Nalezene cislo je: " << (nalezene ? *nalezene : "Nenalezeno") << std::endl;


    for (const auto& prvek : cisla2) {
        // will fail
//        auto d = std::get<std::string>(prvek.second);
//        std::cout << d << std::endl;

        // dont use this
//        try {
//            double d = std::get<double>(prvek.second);
//            std::cout << d << std::endl;
//        }
//        catch (std::bad_variant_access va) {
//            std::string d = std::get<std::string>(prvek.second);
//            std::cout << d << std::endl;
//        }

        std::visit([](auto&& v) {
            if constexpr (std::is_same_v<decltype(v), double>) {
                std::cout << "DOUBLE = ";
            }
            std::cout << v << std::endl;

        }, prvek.second);
    }

    for (const auto& prvek : cisla3) {

        try {
            auto s = std::any_cast<const char*>(prvek.second);
            std::cout << s << std::endl;
        }
        catch (std::bad_any_cast baa) {
            auto s = std::any_cast<double>(prvek.second);
            std::cout << s << std::endl;
        }
    }

    return EXIT_SUCCESS;
}