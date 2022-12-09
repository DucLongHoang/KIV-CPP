#pragma once

#include <vector>
#include <concepts>

template<typename T>
concept ValidArgument = std::is_integral_v<T> &&
                        std::is_unsigned_v<T> &&
                        sizeof(T) >= 4;

template<typename T>
concept Unlimited = true;

template<typename T>
requires ValidArgument<T> or Unlimited<T>
class MPInt {
    private:
        std::vector<T> mNumber;

    public:
        MPInt() = default;
        ~MPInt() = default;

        MPInt& operator+(MPInt& other);
        MPInt& operator-(MPInt& other);
        MPInt& operator*(MPInt& other);
        MPInt& operator/(MPInt& other);
        MPInt& operator+=(MPInt& other);
        MPInt& operator-=(MPInt& other);
        MPInt& operator*=(MPInt& other);
        MPInt& operator/=(MPInt& other);
        MPInt& operator!();

};

