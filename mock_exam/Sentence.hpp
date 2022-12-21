#pragma once

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <concepts>
#include <unordered_map>
//#include "Transformer.hpp"

// forward declaration
class Sentence;

template<typename T>
concept Transformer = requires (T t, const char ch) {
    { t.transform(ch) } -> std::convertible_to<char>;
};

enum class Form {
    DECLARATIVE, INTERROGATIVE, IMPERATIVE
};

class Sentence {
    private:
        static inline const std::unordered_map<Form, char> mPunctuationMarks = {
                { Form::DECLARATIVE, '.' },
                { Form::INTERROGATIVE, '?' },
                { Form::IMPERATIVE, '!' },
        };

        Form mForm;
        std::vector<char> mChars;

    public:
        // constructors
        explicit Sentence(Form form) : mForm(form) {};
        Sentence(Form form, std::vector<char> chars) : mForm(form), mChars(std::move(chars)) {}
        // copy constructor
        Sentence(const Sentence& other) {
            mForm = other.mForm;
            mChars = other.mChars;
        }
        // copy assignment
        Sentence& operator=(const Sentence& other) {
            mForm = other.mForm;
            mChars = other.mChars;
            return *this;
        }
        // move constructor
        Sentence(Sentence&& other) noexcept : mForm(other.mForm), mChars(std::move(other.mChars)) {}
        // move assignment
        Sentence& operator=(Sentence&& other) noexcept {
            mForm = other.mForm;
            mChars = std::move(other.mChars);
            return *this;
        }

        Form getForm() const {
            return mForm;
        }

    Sentence& operator+=(const std::vector<char>& word) {
            mChars.insert(mChars.end(), word.begin(), word.end());
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& stream, const Sentence& sentence) {
            std::for_each(sentence.mChars.begin(), sentence.mChars.end(), [&stream](auto ch) {
                stream << ch;
            });
            if (mPunctuationMarks.contains(sentence.mForm))
                stream << mPunctuationMarks.at(sentence.mForm);
            else stream << '.';

            return stream;
        }

        template<Transformer T>
        Sentence transform(T t) const {
            Sentence result(*this);
            for (auto ch : mChars)
                result.mChars.push_back(t.transform((ch)));

            return result;
        }

        size_t get_word_count() {
            size_t wc = 1;

            for (auto ch : mChars) {
                if (ch == '?' || ch == '.' || ch == '!' || ch == ' ' || ch == ',')
                    wc++;
            }
            return wc;
        }

        size_t get_char_count() {
            return mChars.size();
        }
};