#pragma once

#include <regex>
#include <cctype>
#include <string>
#include <unordered_map>

static const std::regex REGEX_LETTERS("[a-zA-Z]");

class ITransformer {
    public:
        virtual char transform(char ch) const = 0;
};

class UpperCaseTransformer : public ITransformer {
    public:
        UpperCaseTransformer() = default;
        char transform(char ch) const override {
            return static_cast<char>(toupper(ch));
        }
};

class AlphabetShiftTransformer : public ITransformer {
    public:
        AlphabetShiftTransformer() = default;
        char transform(char ch) const override {
            std::string s; s += ch;
            if (!std::regex_match(s, REGEX_LETTERS)) return ch;
            else if (ch == 'z') return 'a';
            else if (ch == 'Z') return 'A';
            else return static_cast<char>(ch + 1);
        }
};