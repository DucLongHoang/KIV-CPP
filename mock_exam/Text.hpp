#pragma once

#include <numeric>
#include <algorithm>
#include <ranges>
#include "Sentence.hpp"

class Text {
    private:
        std::vector<Sentence> mSentences;

    public:
        Text() = default;

        Text& operator+=(const Sentence& sent) {
            mSentences.push_back(sent);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& stream, const Text& text) {
            std::for_each(text.mSentences.begin(), text.mSentences.end(), [&stream](const auto& sent) {
                stream << sent;
            });
            return stream;
        }

        Text& operator+=(const std::string& str) {
            std::vector<char> chars(str.begin(), str.end());
            mSentences.back()+=chars;
            return *this;
        }

        template<Transformer T>
        Text transform_all(T t) {
            Text result;
            for (const auto& sent : mSentences) {
                result.mSentences.push_back(sent.transform(t));
            }
            return result;
        }

        size_t get_word_count() {
            return std::accumulate(mSentences.begin(), mSentences.end(), 0, [](size_t total, Sentence& sent) {
                return total + sent.get_word_count();
            });
        }

        size_t get_char_count() {
            return std::accumulate(mSentences.begin(), mSentences.end(), 0, [](size_t total, Sentence& sent) {
                return total + sent.get_char_count();
            });
        }

        size_t get_sentence_form_count(Form f) {
            auto sentence_form = [&f](const Sentence& s) { return s.getForm() == f; };
            auto v = mSentences | std::views::filter(sentence_form);
            return v.size();
        }
};