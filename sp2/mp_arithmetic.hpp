#pragma once

#include <limits>
#include <vector>
#include <sstream>
#include <concepts>

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
        static constexpr unsigned char MAX_DIGIT = std::numeric_limits<unsigned char>::max();
        // describes a number in base 255, which is stored in reverse order
        std::vector<unsigned char> mNumber;
        bool mIsNegative;

    public:
        // constructor
        explicit MPInt(long long init) {
            mIsNegative = init < 0;
            init = std::abs(init);
            while (init) {
                mNumber.push_back(init % MAX_DIGIT);
                init /= MAX_DIGIT;  // carry
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

        template<size_t U> requires AtLeast4Bytes<U>
        friend MPInt<std::max(T, U)> operator+(MPInt& lhs, MPInt<U>& rhs) {
            // result to be returned
            MPInt<std::max(T, U)> result(0);
            // make vectors same length
            while (rhs.mNumber.size() > lhs.mNumber.size()) {
                lhs.mNumber.push_back(0);
            }
            // calculation
            unsigned int sum = 0;
            for (size_t i = 0; i < lhs.mNumber.size(); ++i) {
                sum += (lhs.mNumber[i] + rhs.mNumber[i]);
                result.mNumber.push_back(sum % 255);
                sum /= 255;   // = carry
            }
            return result;
        }

        MPInt operator-(const MPInt& other) const;
        MPInt operator*(const MPInt& other) const;
        MPInt operator/(const MPInt& other) const;

        template<size_t U> requires AtLeast4Bytes<U>
        MPInt<std::max(T, U)> operator+=(const MPInt<U>& other) {
            while (other.mNumber.size() > this->mNumber.size()) {
                this->mNumber.push_back(0);
            }
            unsigned int sum = 0;
            for (size_t i = 0; i < other.size(); ++i) {
                sum += (this->mNumber[i] + other.mNumber[i]);
                this->mNumber.push_back(sum % MAX_DIGIT);
                sum /= MAX_DIGIT;
            }
            return *this;
        }

        MPInt& operator-=(const MPInt& other);
        MPInt& operator*=(const MPInt& other);
        MPInt& operator/=(const MPInt& other);
        MPInt operator!() const;

        template<size_t U>
        requires AtLeast4Bytes<U>
        friend std::ostream& operator<< (std::ostream& os, const MPInt<U>& num);

};

template<size_t T>
requires AtLeast4Bytes<T>
std::ostream &operator<<(std::ostream &os, const MPInt<T> &num) {
    std::stringstream stream{};
    size_t carry = 0;
    std::for_each(num.mNumber.begin(), num.mNumber.end(), [&carry, &stream](unsigned char i) {
        stream << i;
    });
    return os;
}

