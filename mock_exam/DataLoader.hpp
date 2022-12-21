#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <filesystem>
#include "Text.hpp"

class DataLoader {
    private:
        Text text;

    public:
        DataLoader() = default;

        Text& get_text() {
            return text;
        }

        void load(const std::string& str) {
            try {
                if (!std::filesystem::exists(str)) throw std::runtime_error("File does not exist.");
                if (std::filesystem::is_directory(str)) throw std::runtime_error("File is a directory.");

                std::ifstream in(str);
                char ch;
                std::vector<char> chars;
                while (in >> std::noskipws >> ch) {
                    switch (ch) {
                        case '.': {
                            text += Sentence(Form::DECLARATIVE, chars);
                            chars.clear();
                            break;
                        }
                        case '?': {
                            text += Sentence(Form::INTERROGATIVE, chars);
                            chars.clear();
                            break;
                        }
                        case '!': {
                            text += Sentence(Form::IMPERATIVE, chars);
                            chars.clear();
                            break;
                        }
                        default: {
                            chars.emplace_back(ch);
                            break;
                        }
                    }
                }
            }
            catch (std::runtime_error& er) {
                std::cerr << er.what() << std::endl;
            }
        }
};