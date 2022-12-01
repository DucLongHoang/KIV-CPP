#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <string_view>
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

enum class Filetype {
    TEXT, IMAGE, UNKNOWN
};

// using string literals
using namespace std::string_literals;
using file = std::tuple<std::string, Filetype, unsigned int>;

std::map<Filetype, std::string> filetypeMap = {
        {Filetype::TEXT, "TEXT"s},
        {Filetype::IMAGE, "IMAGE"s},
//        {Filetype::UNKNOWN, "UNKNOWN"s},
};

// using optional
std::optional<std::string> get_filetype(Filetype ft) {
    if (filetypeMap.contains(ft))
        return filetypeMap[ft];
    return std::nullopt;
}

// using regexes
const std::regex textFiles(".(txt|pdf|word)");
const std::regex imageFiles(".(png|bmp|jpg|jpeg)");

int main(int argc, char** argv) {
    auto rootDir = "s:/Git/UWB/KIV-PRO/"s;
    // using filesystem
    auto fileWalker = std::filesystem::recursive_directory_iterator(rootDir);   // lol
    std::vector<file> files;

    for (const auto& dirEntry : fileWalker) {
        if (dirEntry.is_directory()) continue;

        const auto& path = dirEntry.path();

        Filetype ft = Filetype::UNKNOWN;
        if (std::regex_match(path.extension().string(), textFiles)) ft = Filetype::TEXT;
        else if (std::regex_match(path.extension().string(), imageFiles)) ft = Filetype::IMAGE;

        files.emplace_back(path.filename().string(), ft, dirEntry.file_size());
    }

    for (const file& f : files) {
        // using structured binding
        auto[name, filetype, size] = f;
        auto ft = get_filetype(filetype);

        std::cout << "Filename:  " << name << std::endl;
        std::cout << "File type: " << (ft ? ft.value() : "UNKNOWN") << std::endl;
        std::cout << "File size: " << size << " B" << std::endl;
        std::cout << "-----------" << std::endl;
    }

    return EXIT_SUCCESS;
}