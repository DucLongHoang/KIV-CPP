#pragma once

#include <map>
#include <array>
#include <deque>
#include <regex>
#include <string>
#include <variant>
#include <optional>
#include <algorithm>
#include <functional>
#include "mp_arithmetic.hpp"

// type aliases
using Operands = std::pair<std::string, std::string>;
using Handler = std::function<bool (const Operands&)>;

// recognized math operations
static constexpr std::array<char, 5> MATH_OPS{'+', '-', '*', '/', '!'};
static const std::regex REGEX_MATH_OPS(R"([\+\-\*\/\!])");
static const std::regex REGEX_NUMBER("[0-9]+");
static const std::regex REGEX_BANK("^\\$[1-5]{1}");

/**
 *
 * @tparam T
 */
template<size_t T> requires AtLeast4Bytes<T>
class MPTerm {
    private:
        static constexpr size_t BANK_LIMIT = 5;

        std::deque<MPInt<T>> mBank;
        std::map<std::string, Handler> mHandlerMap;

        void bank_number(MPInt<T> num);
        void fill_handlers();

    public:
        // constructor
        explicit MPTerm() {
            mBank = std::deque<MPInt<T>>{};
            fill_handlers();
        }
        void run();
        std::optional<std::pair<MPInt<T>, MPInt<T>>> get_operands(const Operands& ops);
        std::optional<MPInt<T>> get_operand(const std::string& str);
};

template<size_t T> requires AtLeast4Bytes<T>
std::optional<MPInt<T>> MPTerm<T>::get_operand(const std::string& str) {
    std::variant<long long int, MPInt<T>> var;
    MPInt<T> op(0);

    // get operand from number or bank
    if (std::regex_match(str, REGEX_BANK)){
        size_t idx = std::stoi(str.substr(1)) - 1;  // $1 = mBank[0]
        if (idx > (mBank.size() - 1)) {     // index not in bank yet
            std::cout << "$" << str.substr(1) << " not in bank yet" << std::endl;
            return std::nullopt;
        }
        else var = mBank[idx];
    }
    else if (std::regex_match(str, REGEX_NUMBER))
        var = MPInt<T>(std::stoull(str));
    else return std::nullopt;

    // use correct operand from variant
    std::visit([&op](auto&& val) { op = MPInt<T>(val); }, var);
    return op;
}

template<size_t T> requires AtLeast4Bytes<T>
std::optional<std::pair<MPInt<T>, MPInt<T>>> MPTerm<T>::get_operands(const Operands &ops) {
    auto opt1 = get_operand(ops.first);
    auto opt2 = get_operand(ops.second);
    if (!opt1 || !opt2) return std::nullopt;

    return std::make_pair(opt1.value(), opt2.value());
}

template<size_t T> requires AtLeast4Bytes<T>
void MPTerm<T>::bank_number(MPInt<T> num) {
    mBank.push_front(num);
//    mBank.resize(std::min(mBank.size(), BANK_LIMIT));     // does not work, don't know why
    if (mBank.size() > BANK_LIMIT) mBank.pop_back();
}

template<size_t T> requires AtLeast4Bytes<T>
void MPTerm<T>::fill_handlers() {
    mHandlerMap["+"] = [this](const Operands &op) -> bool {
        auto opt = get_operands(op);
        if (!opt) return true;

        auto [lhs, rhs] = opt.value();
        MPInt<T> result = lhs + rhs;
        if (result.overflowed()) return true;

        std::cout << "$1 = " << result << std::endl;
        bank_number(result);
        return true;
    };

    mHandlerMap["-"] = [this](const Operands &op) -> bool {
        auto opt = get_operands(op);
        if (!opt) return true;

        auto [lhs, rhs] = opt.value();
        MPInt<T> result = lhs - rhs;
        if (result.overflowed()) return true;

        std::cout << "$1 = " << result << std::endl;
        bank_number(result);
        return true;
    };

    mHandlerMap["*"] = [this](const Operands &op) -> bool {
        auto opt = get_operands(op);
        if (!opt) return true;

        auto [lhs, rhs] = opt.value();
        MPInt<T> result = lhs * rhs;
        if (result.overflowed()) return true;

        std::cout << "$1 = " << result << std::endl;
        bank_number(result);
        return true;
    };

    mHandlerMap["/"] = [this](const Operands &op) -> bool {
        auto opt = get_operands(op);
        if (!opt) return true;

        auto [lhs, rhs] = opt.value();
        MPInt<T> result = lhs / rhs;
        if (result.overflowed()) return true;

        std::cout << "$1 = " << result << std::endl;
        bank_number(result);
        return true;
    };

    mHandlerMap["!"] = [this](const Operands &op) -> bool {
        auto opt = get_operand(op.first);
        if (!opt) return true;

        MPInt<T> operand = opt.value();
        MPInt<T> result = operand.fact();
        if (result.overflowed()) return true;

        std::cout << "$1 = " << result << std::endl;
        bank_number(result);
        return true;
    };

    mHandlerMap["bank"] = [this](const Operands &op) -> bool {
        size_t i = 1;
        for (const auto& num : mBank) {
            std::cout << "$" << i++ << " = " << num << std::endl;
        }
        return true;
    };

    mHandlerMap["exit"] = [](const Operands &op) -> bool {
        return false;
    };
}

template<size_t T> requires AtLeast4Bytes<T>
void MPTerm<T>::run() {
    std::string command, operation;
    Operands operands;
    bool ret = true;

    while(ret) {
        std::cout << ">";
        std::getline(std::cin, command);
        operation = command;    // set default

        size_t operatorCount = 0;
        for (auto c : command) {
            if (std::regex_match(std::string{c} , REGEX_MATH_OPS)) operatorCount++;
        }

        if (operatorCount == 1) {
            // too lazy to implement shunting yard algorithm
            for (const auto op : MATH_OPS) {
                // finding math operator
                size_t pos = command.find_first_of(op);
                if (pos != std::string::npos && pos != 0) {
                    // expression 100+$2 is converted to -> operation = "+" and operands = "100,$2"
                    operation = op;
                    operands.first = command.substr(0, pos);
                    operands.second = command.substr(pos + 1);
                    break;
                }
            }
        }
        // check if command exists
        if (!mHandlerMap.contains(operation)) {
            std::cout << "Invalid command: " << operation << std::endl;
            continue;
        }
        ret = mHandlerMap[operation](operands);
    }
}