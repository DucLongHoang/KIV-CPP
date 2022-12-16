#pragma once

#include <cmath>
#include <limits>
#include <ranges>
#include <vector>
#include <compare>
#include <iomanip>
#include <sstream>
#include <utility>
#include <concepts>
#include <stdexcept>
#include <type_traits>

// base
static constexpr unsigned short BASE = 10000;
static constexpr size_t MPI_UNLIMITED = std::numeric_limits<size_t>::max();

/**
 * Concept AtLeast4Bytes - self-explanatory
 * @tparam T positive integer
 */
template<size_t T>
concept AtLeast4Bytes = T >= 4;

/**
 * Template class MPInt - multiple precision integer
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
                explicit MPIntException(std::string str) : mNum(MPInt(0)), mStr(std::move(str)) {}
                const char* what() noexcept {
                    std::stringstream ss;
                    ss << mNum;
                    mStr.append(ss.str());
                    return mStr.c_str();
                }
        };

        /**
         * Makes vectors same length by padding the shorter one with zeroes.
         * @tparam U template parameter
         * @param lhs first number
         * @param rhs second number
         */
        template<size_t U> requires AtLeast4Bytes<U>
        static void add_padding(MPInt& lhs, MPInt<U>& rhs)  {
            while (lhs.mDigits.size() < rhs.mDigits.size()) lhs.mDigits.push_back(0);
            while (lhs.mDigits.size() > rhs.mDigits.size()) rhs.mDigits.push_back(0);
        }

        /**
         * Removes zero padding added by the add_padding() method.
         * @return class instance
         */
        MPInt& remove_padding()  {
            while (mDigits.size() > 1 && mDigits.back() == 0)
                mDigits.pop_back();
            return *this;
        }

        /**
         * Halves this number
         * @return class instance
         */
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

        /**
         * Spaceship operator for all comparisons
         * @tparam U template parameter
         * @param other number to compare against
         * @return some std::strong_ordering
         */
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

        /**
         * Ok, so spaceship operator was not enough...
         * It would be enough if it was defaulted.
         * @tparam U template parameter
         * @param other number to compare against
         * @return true if equal else false
         */
        template<size_t U> requires AtLeast4Bytes<U>
        bool operator==(const MPInt<U> other) const {
            if (mDigits.size() != other.mDigits.size()) return false;

            for (int i = (mDigits.size() - 1); i >= 0; i-- ) {
                bool comp = (mDigits[i] == other.mDigits[i]);
                if (!comp) return false;
            }
            return true;
        }


        /**
         * Checks if number is is_bad. If yes throws an exception.
         * @return true if overflown (+ throw exception) else false
         */
        [[nodiscard]] bool is_bad() const {
            // zero division check
            if (*this == MPInt(0) && mIsNegative) {
                try { throw MPIntException("Division by: "); }
                catch (MPIntException& mpIntException) {
                    std::cout << mpIntException.what() << std::endl;
                    return true;
                }
            }
            // overflow check
            if (T == MPI_UNLIMITED) return false;

            long long int max = std::pow(2, (T * 8));
            if (*this > MPInt(max)) {
                try { throw MPIntException(*this); }
                catch (MPIntException& mpIntException) {
                    std::cout << mpIntException.what() << std::endl;
                    return true;
                }
            }
            return false;
        }

        /**
         * Calculates factorial
         * @return factorial of this number
         */
        MPInt fact() {
            MPInt it(*this), one(1), zero(0);
            if (*this == zero) return one;

            while (it > one) {
                it = it - one;
                *this = *this * it;
            }
            return *this;
        }

        // Overloaded operators

        /**
         * Returns sum of 2 numbers
         * @tparam U template parameter
         * @param lhs first operand
         * @param rhs second operand
         * @return sum of operands with the higher of the two precisions
         */
        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator+(MPInt& lhs, MPInt<U>& rhs) {
            // if only one operand is negative
            if (lhs.mIsNegative != rhs.mIsNegative)
                return (lhs.mIsNegative) ? (rhs - !lhs) : (lhs - !rhs);

            // result to be returned
            MPInt<std::max(T, U)> result(0), zero(0);
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
            if (lhs == zero) lhs.mIsNegative = false;
            return result;
        }

        /**
         * Returns subtraction result of 2 numbers
         * @tparam U template parameter
         * @param lhs first operand
         * @param rhs second operand
         * @return subtraction of operands with the higher of the two precisions
         */
        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator-(MPInt& lhs, MPInt<U>& rhs) {
            // if only one operand is negative
            if (lhs.mIsNegative != rhs.mIsNegative)
                return (lhs.mIsNegative) ? (!(!lhs + rhs)) : (lhs + !rhs);

            // result to be returned
            MPInt<std::max(T, U)> result(0), zero(0);
            result.mDigits.pop_back();  // empty out the result first

            bool swap = false;
            // some magic
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
            if (lhs == zero) lhs.mIsNegative = false;
            return result;
        }


        /**
         * Returns multiplication result of 2 numbers
         * @tparam U template parameter
         * @param lhs first operand
         * @param rhs second operand
         * @return multiplication result of operands with the higher of the two precisions
         */
        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator*(MPInt& lhs, MPInt<U>& rhs) {
            MPInt<std::max(T, U)> a(lhs), b(rhs), zero(0);
            lhs = MPInt(0);
            bool flip = false;

            // multiplying with 1 negative operand
            if (a.mIsNegative != b.mIsNegative) {
                flip = true;
            }
            // both operands are negative or positive
            else if (a.mIsNegative) {   // flip if negative
                !a, !b;
            }
            while (a > zero) {  // some magic
                if (a.mDigits.front() % 2) {
                    lhs = lhs + b;
                }
                a.halve();
                b = b + b;
            }
            if (flip) lhs.mIsNegative = true;
            return lhs;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator/(MPInt& lhs, MPInt<U>& rhs) {
            MPInt zero(0), one(1);
            bool neg = false;
            // only one of operands is negative
            if (lhs.mIsNegative != rhs.mIsNegative)
                neg = true;
            // both operands negative
            else if (lhs.mIsNegative)
                !lhs, !rhs;

            // magic
            if (rhs == zero) return !zero;
            if (lhs == zero) return zero;
            if (rhs == one) {
                lhs.mIsNegative = neg;
                return lhs;
            }
            if (rhs == lhs) {
                one.mIsNegative = neg;
                return one;
            }
            if (rhs > lhs) return zero;

            // very stupid calculation
            MPInt<std::max(T, U)> quotient(0);
            lhs.mIsNegative = false;
            rhs.mIsNegative = false;

            MPInt tmp = lhs - rhs;
            while (tmp >= zero && !tmp.mIsNegative) {
                quotient = quotient + one;
                tmp = tmp - rhs;
            }

            quotient.mIsNegative = neg;
            return quotient;
        }

        /**
         * Same as x + y
         * @tparam U template parameter
         * @param other number to sum with
         * @return sum
         */
        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator+=(MPInt<U>& other) {
            return *this + other;
        }

        /**
         * Same as x - y
         * @tparam U template parameter
         * @param other number to subtract
         * @return subtraction result
         */
        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator-=(const MPInt<U>& other) {
            return *this - other;
        }

        /**
         * Same as x * y
         * @tparam U template parameter
         * @param other number to multiply by
         * @return multiplication result
         */
        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator*=(const MPInt<U>& other) {
            return *this * other;
        }

        /**
         * Same as x / y
         * @tparam U template parameter
         * @param other number to divide by
         * @return division result
         */
        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator/=(const MPInt<U>& other) {
            return *this / other;
        }

        /**
         * Flips the +/- sign of this number
         * @return class instance
         */
        MPInt& operator!() {
            mIsNegative = !mIsNegative;
            return *this;
        }

        /**
         * "Getter" for mIsNegative
         * @return mIsNegative
         */
        explicit operator bool() {
            return mIsNegative;
        }

        /**
         * Printing to ostream
         * @param os stream to output to
         * @param num to output
         * @return stream so it can be chained
         */
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