#include <iostream>

class Vector2D {
private:
    double mX = 0;
    double mY = 0;

public:
    Vector2D() = default;
    explicit Vector2D(double x, double y) : mX(x), mY(y) {}
    double get_x() const {
        return mX;
    }
    double get_y() const {
        return mY;
    }

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D{this->get_x() + other.get_x(), this->get_y() + other.get_y()};
    }

    Vector2D& operator+=(const Vector2D& other) {
        mX += other.get_x();
        mY += other.get_y();
        return *this;
    }

    double operator*(const Vector2D& other) const {
        return this->get_x()*other.get_x() + this->get_y()*other.get_y();
    }

    friend std::ostream& operator<<(std::ostream& os, Vector2D& v);
};


std::ostream& operator<<(std::ostream& os, Vector2D& v) {
    os << "Vektor ma slozky x = " << v.mX << " a y = " << v.mY;
    return os;
}

int main() {
    Vector2D a{2.5, 5.1};
    Vector2D b{3, 5.8};

    double soucin = a * b;
    std::cout << "Soucin je: " << soucin << std::endl;

    Vector2D c = a + b;
    std::cout << c << std::endl;
}

