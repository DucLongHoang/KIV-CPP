#pragma once

#include <limits>
#include <vector>
#include <iomanip>
#include <sstream>
#include <concepts>
#include <type_traits>

// base
static constexpr unsigned char BASE = 100;

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
        std::vector<unsigned char> mNumber;
        bool mIsNegative;

    public:
        // constructor
        explicit MPInt(long long init) {
            mIsNegative = init < 0;
            if (init == 0) mNumber.push_back(0);
            else {
                init = std::abs(init);
                while (init) {
                    mNumber.push_back(init % BASE);
                    init /= BASE;  // carry
                }
            }
        }
        // copy constructor
        MPInt(const MPInt& o) : mNumber(o.mNumber), mIsNegative(o.mIsNegative) {}
        // copy assignment
        MPInt& operator=(const MPInt& o){
            mNumber = o.mNumber;
            mIsNegative = o.mIsNegative;
            return *this;
        }
        // move constructor
        MPInt(MPInt&& o) noexcept : mNumber(std::move(o.mNumber)), mIsNegative(std::move(o.mIsNegative)) {}
        // move assignment
        MPInt& operator=(MPInt&& o) noexcept {
            mNumber = std::move(o.mNumber);
            mIsNegative = std::move(o.mIsNegative);
            return *this;
        }
        // destructor
        ~MPInt() = default;

        // public iterators for range related stuffs
        [[nodiscard]] std::vector<unsigned char>::const_iterator begin() const { return mNumber.begin(); }
        [[nodiscard]] std::vector<unsigned char>::const_iterator end() const { return mNumber.end(); }

        // make vectors same length
        template<size_t U> requires AtLeast4Bytes<U>
        static void add_padding(MPInt& lhs, MPInt<U>& rhs)  {
            while (lhs.mNumber.size() < rhs.mNumber.size()) lhs.mNumber.push_back(0);
            while (lhs.mNumber.size() > rhs.mNumber.size()) rhs.mNumber.push_back(0);
        }

        // return vector length to original
        template<size_t U> requires AtLeast4Bytes<U>
        static void remove_padding(MPInt& lhs, MPInt<U>& rhs)  {
            while (lhs.mNumber.back() == 0) lhs.mNumber.pop_back();
            while (rhs.mNumber.back() == 0) rhs.mNumber.pop_back();
        }


        // overloaded operators

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator+(MPInt& lhs, MPInt<U>& rhs) {
            // result to be returned
            MPInt<std::max(T, U)> result(0);
            result.mNumber.pop_back();  // empty out the result first
            add_padding(lhs, rhs);
            // calculation
            unsigned char sum = 0;
            for (size_t i = 0; i < rhs.mNumber.size(); ++i) {
                sum += (lhs.mNumber[i] + rhs.mNumber[i]);
                result.mNumber.push_back(sum % BASE);
                sum /= BASE;   // = carry
            }
            if (sum != 0) result.mNumber.push_back(sum);    // push last carry
            remove_padding(lhs, rhs);
            return result;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt operator-(MPInt& lhs, MPInt<U>& rhs) {
            // result to be returned
            MPInt<std::max(T, U)> result(0);
            result.mNumber.pop_back();  // empty out the result first

            // make vectors same length
            while (lhs.mNumber.size() < rhs.mNumber.size()) lhs.mNumber.push_back(0);
            while (lhs.mNumber.size() > rhs.mNumber.size()) rhs.mNumber.push_back(0);

            // calculation
            short diff = 0;
            for (size_t i = 0; i < rhs.mNumber.size(); ++i) {
                diff += (lhs.mNumber[i] - rhs.mNumber[i]);
                result.mNumber.push_back(diff % BASE);
                diff /= BASE;   // = carry
            }
            if (diff != 0) result.mNumber.push_back(diff);    // push last carry
            return result;
        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt operator*(MPInt& lhs, MPInt<U>& rhs) {

        }

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt operator/(MPInt& lhs, MPInt<U>& rhs) {

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

        }

        friend std::ostream& operator<< (std::ostream& os, const MPInt& num) {
            std::stringstream ss;

            ss << static_cast<unsigned int>(num.mNumber.back());    // first number is not padded
            std::for_each(num.mNumber.rbegin() + 1, num.mNumber.rend(), [&ss](unsigned char i) {
                ss << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(i);
            });
            std::reverse(ss.str().begin(), ss.str().end());
            return os << ss.str();
        }
};