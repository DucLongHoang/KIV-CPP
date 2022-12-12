#pragma once

#include <map>
#include <deque>
#include <array>
#include <regex>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include "mp_arithmetic.hpp"

using Operands = std::pair<std::string, std::string>;
using Handler = std::function<bool (const Operands&)>;

/**
 *
 * @tparam T
 */
template<size_t T>
requires AtLeast4Bytes<T>
class MPTerm {
    private:
        static constexpr size_t BANK_LIMIT = 5;
        static constexpr std::array<char, 5> MATH_OP {'+', '-', '*', '/', '!'};

        std::deque<MPInt<T>> mBank;
        std::map<std::string, Handler> mHandlerMap;

        void bank_number(MPInt<T> num) {
            mBank.push_front(num);
            mBank.resize(std::min(mBank.size(), BANK_LIMIT));
        }

        void fill_handlers() {
            mHandlerMap["+"] = [this](const Operands &op) -> bool {
                MPInt<T> lhs(std::stoull(op.first));
                MPInt<T> rhs(std::stoull(op.second));
                MPInt<T> result = lhs + rhs;
                std::cout << "$1 = " << result << std::endl;
                bank_number(std::move(result));

//                mBank.push_front(result);

                return true;
            };

            mHandlerMap["-"] = [](const Operands &op) -> bool {

                return true;
            };

            mHandlerMap["*"] = [](const Operands &op) -> bool {

                return true;
            };

            mHandlerMap["/"] = [](const Operands &op) -> bool {

                return true;
            };

            mHandlerMap["!"] = [](const Operands &op) -> bool {

                return true;
            };

            mHandlerMap["bank"] = [this](const Operands &op) -> bool {
                int i = 1;
//                std::for_each(mBank.begin(), mBank.end(), [&i](MPInt<T> num) {
//                    std::cout << "$" << i << " = " << num << std::endl;
//                });
                return true;
            };

            mHandlerMap["exit"] = [](const Operands &op) -> bool {
                return false;
            };
        }


    public:
        // constructor
        explicit MPTerm() {
            mBank = std::deque<MPInt<T>>{};
            fill_handlers();
        }
        void run();
};


template<size_t T>
requires AtLeast4Bytes<T>
void MPTerm<T>::run() {
    std::string command, operation;
    Operands operands;
    bool ret = true;
    while(ret) {
        std::cout << "> ";
        std::getline(std::cin, command);
        operation = command;    // set default

        // too lazy to implement shunting yard algorithm
        for (const auto& op : MATH_OP) {
            // finding math operator
            size_t pos = command.find_first_of(op);
            if (pos != std::string::npos) {
                operands.second = command.substr(pos + 1);

                // necessary cast to pass to lambda
                auto secondOp = std::ref(operands.second);

                // check for other operators in rest of command
                bool moreOps = std::any_of(MATH_OP.begin(), MATH_OP.end(), [secondOp](char op) {
                    return secondOp.get().find(op) != std::string::npos;
                });
                if (moreOps) break;

                // expression 100+$2 is converted to ->
                // operation = "+"
                // args = "100,$2"
                operation = op;
                operands.first = command.substr(0, pos);
                break;
            }
        }
        // check if command exists
        if (!mHandlerMap.contains(operation)) {
            std::cout << "Neplatne zadani/prikaz: " << operation << std::endl;
            continue;
        }
        ret = mHandlerMap[operation](operands);
    }
}
