#include <iostream>
#include "Text.hpp"
#include "DataLoader.hpp"

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Incorrect number of args" << std::endl;
        exit(-1);
    }

    DataLoader loader;
    const auto filename = std::string(argv[1]);
    loader.load(filename);

    Text t = loader.get_text();
//    Text up = t.transform_all<Transformer>();
//    Text shift = t.transform_all<Transformer>();

    std::cout << "The text has: " << t.get_char_count() << " characters" << std::endl;
    std::cout << "and about: " << t.get_word_count() << " words (+/-)." << std::endl;
    std::cout << std::endl;

    t+="amen";
    std::cout << t << std::endl;

    //    std::cout << up << std::endl;
//    std::cout << shift << std::endl;

    return EXIT_SUCCESS;
}