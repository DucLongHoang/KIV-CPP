#pragma once

#include <cmath>
#include <limits>
#include <ranges>
#include <vector>
#include <compare>
#include <iomanip>
#include <sstream>
#include <concepts>
#include <stdexcept>
#include <type_traits>

// base
static constexpr unsigned short BASE = 10000;
static constexpr size_t MPI_UNLIMITED = std::numeric_limits<size_t>::max();

/**
 * Concept AtLeast4Bytes - self-explanatory
 * @tparam T
 */
template<size_t T>
concept AtLeast4Bytes = T >= 4;

/**
 * Template class MPInt
 * @tparam T expects a number which describes the precision in bytes
 */
template<size_t T>
requires AtLeast4Bytes<T>
class MPInt {
    private:
        // describes a number in base BASE, which is stored in reverse order
        // the least significant digit at [0]
        std::vector<unsigned short> mDigits;
        bool mIsNegative;


    public:
        // constructor
        explicit MPInt(long long int init) {
            mIsNegative = init < 0;
            if (init == 0) mDigits.push_back(0);
            else {
                init = std::abs(init);
                while (init) {
                    mDigits.push_back(init % BASE);
                    init /= BASE;  // carry
                }
            }
        }
        // copy constructor
        MPInt(const MPInt& o) : mDigits(o.mDigits), mIsNegative(o.mIsNegative) {}
        // copy assignment
        MPInt& operator=(const MPInt& o){
            mDigits = o.mDigits;
            mIsNegative = o.mIsNegative;
            return *this;
        }
        // move constructor
        MPInt(MPInt&& o) noexcept : mDigits(std::move(o.mDigits)), mIsNegative(std::move(o.mIsNegative)) {}
        // move assignment
        MPInt& operator=(MPInt&& o) noexcept {
            mDigits = std::move(o.mDigits);
            mIsNegative = std::move(o.mIsNegative);
            return *this;
        }
        // destructor
        ~MPInt() = default;

        // exception class
        class MPIntException : public std::exception {
            private:
                std::string mStr;
                MPInt mNum;

            public:
                explicit MPIntException(const MPInt& num) : mNum(num), mStr("Overflown number: ") {}
                const char* what() noexcept {
                    std::stringstream ss;
                    ss << mNum;
                    mStr.append(ss.str());
                    return mStr.c_str();
                }
        };

        // make vectors same length
        template<size_t U> requires AtLeast4Bytes<U>
        static void add_padding(MPInt& lhs, MPInt<U>& rhs)  {
            while (lhs.mDigits.size() < rhs.mDigits.size()) lhs.mDigits.push_back(0);
            while (lhs.mDigits.size() > rhs.mDigits.size()) rhs.mDigits.push_back(0);
        }

        // return vector length to original
        MPInt& remove_padding()  {
            while (mDigits.size() > 1 && mDigits.back() == 0)
                mDigits.pop_back();
            return *this;
        }

        MPInt& halve() {
            size_t carry = 0;
            for (auto& i : std::ranges::reverse_view(mDigits)) {
                carry *= BASE;
                carry += i;
                i = carry / 2;
                carry %= 2;
            }
            return this->remove_padding();
        }

        // spaceship operator for all comparisons
        template<size_t U> requires AtLeast4Bytes<U>
        auto operator <=>(const MPInt<U> other) const {
            if (mDigits.size() < other.mDigits.size())
                return std::strong_ordering::less;
            else if (mDigits.size() > other.mDigits.size())
                return std::strong_ordering::greater;
            else {
                for (int i = (mDigits.size() - 1); i >= 0; i-- ) {
                    auto comp = (mDigits[i] <=> other.mDigits[i]);
                    if (comp == std::strong_ordering::equal) continue;
                    else return comp;
                }
                return std::strong_ordering::equal;
            }
        }

        // ok, so spaceship operator was not enough
        template<size_t U> requires AtLeast4Bytes<U>
        bool operator==(const MPInt<U> other) const {
            if (mDigits.size() != other.mDigits.size()) return false;

            for (int i = (mDigits.size() - 1); i >= 0; i-- ) {
                bool comp = (mDigits[i] == other.mDigits[i]);
                if (!comp) return false;
            }
            return true;
        }


        // overloaded operators

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator+(MPInt& lhs, MPInt<U>& rhs) {
            // if only one operand is negative
            if (lhs.mIsNegative != rhs.mIsNegative)
                return (lhs.mIsNegative) ? (rhs - !lhs) : (lhs - !rhs);

            // result to be returned
            MPInt<std::max(T, U)> result(0);
            result.mDigits.pop_back();  // empty out the result first
            result.mIsNegative = lhs.mIsNegative;   // adding 2 neg. numbers

            // start calculation
            add_padding(lhs, rhs);
            size_t sum = 0;      // max is 1 + 99 + 99 < unsigned char is 255
            for (size_t i = 0; i < rhs.mDigits.size(); ++i) {
                sum += (lhs.mDigits[i] + rhs.mDigits[i]);
                result.mDigits.push_back(sum % BASE);
                sum /= BASE;   // = carry
            }
            result.mDigits.push_back(sum);  // push last carry
            // end calculation

            lhs.remove_padding();
            rhs.remove_padding();
            result.remove_padding();        //  last carry could be 0
            return result;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator-(MPInt& lhs, MPInt<U>& rhs) {
            // if only one operand is negative
            if (lhs.mIsNegative != rhs.mIsNegative)
                return (lhs.mIsNegative) ? (!(!lhs + rhs)) : (lhs + !rhs);

            // result to be returned
            MPInt<std::max(T, U)> result(0);
            result.mDigits.pop_back();  // empty out the result first

            bool swap = false;
            if (lhs.mIsNegative) {
                if (lhs < rhs) {
                    std::swap(lhs, rhs);
                    swap = true;
                }
                else result.mIsNegative = true;
            }
            else {
                if (lhs < rhs) {
                    result.mIsNegative = true;
                    std::swap(lhs, rhs);
                    swap = true;
                }
            }

            // start calculation
            add_padding(lhs, rhs);
            size_t carry = 0;
            for (size_t i = 0; i < rhs.mDigits.size(); ++i) {
                long long int sub = (lhs.mDigits[i] - rhs.mDigits[i] - carry);
                if (sub < 0) {
                    sub = sub + BASE;
                    carry = 1;
                }
                else carry = 0;
                result.mDigits.push_back(sub);
            }
            // end calculation
            if (swap) std::swap(lhs, rhs);

            lhs.remove_padding();
            rhs.remove_padding();
            result.remove_padding();
            return result;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator*(MPInt& lhs, MPInt<U>& rhs) {
            MPInt<std::max(T, U)> a(lhs), b(rhs);
            MPInt zero(0);
            lhs = MPInt(0);

            // multiplying with 1 negative operand
            if (a.mIsNegative != b.mIsNegative)
                lhs.mIsNegative = true;
            // both operands are negative or positive
            else if (a.mIsNegative) {   // flip if negative
                !a, !b;
            }
            while (a > zero) {
                if (a.mDigits.front() % 2) {
                    lhs = lhs + b;
                }
                a.halve();
                b = b + b;
            }
            return lhs;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator/(MPInt& lhs, MPInt<U>& rhs) {

            MPInt<std::max(T, U)> res(0);
            while (lhs > MPInt(0)) {
                lhs = lhs - rhs;
                MPInt one(1);
                res = res + one;
            }
            return res;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator+=(MPInt<U>& other) {
            return *this + other;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator-=(const MPInt<U>& other) {
            return *this - other;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator*=(const MPInt<U>& other) {
            return *this * other;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator/=(const MPInt<U>& other) {
            return *this / other;
        }

        MPInt& operator!() {
            mIsNegative = !mIsNegative;
            return *this;
        }

        bool overflowed() const {
            if (T == MPI_UNLIMITED) return false;
            long long int max = std::pow(2, (T * 8));
            if (*this > MPInt(max)) {
                try {
                    throw MPIntException(*this);
                }
                catch (MPIntException& mpIntException) {
                    std::cout << mpIntException.what() << std::endl;
                    return true;
                }
            }
            return false;
        }

        MPInt& fact() {
            MPInt it(*this);
            MPInt one(1);
            while (it > one) {
                it = it - one;
                *this = *this * it;
            }
            return *this;
        }

        friend std::ostream& operator<< (std::ostream& os, const MPInt& num) {
            std::stringstream ss;
            ss << (num.mDigits.back());    // first number is not padded

            // pad numbers
            for (long long int i = num.mDigits.size() - 2; i >= 0; --i) {
                ss << std::setfill('0') << std::setw(4) << num.mDigits[i];
            }
            if (num.mIsNegative) os << "-";     // negative number
            return os << ss.str();
        }
};