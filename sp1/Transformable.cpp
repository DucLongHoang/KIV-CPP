#include <algorithm>
#include <cmath>
#include "Transformable.h"

// Start: aux functions

constexpr double PI = 3.14159;

double degree_to_radian(double degree) {
    return degree * (PI / 180);
}

// End: aux functions

// Start: Canvas

void Canvas::translate(double x, double y) {
    std::for_each(mShapes.begin(), mShapes.end(),
                  [x, y](std::unique_ptr<ITransformable>& object) {
        object->translate(x, y);
    });
}

void Canvas::rotate(double x, double y, double angle) {
    std::for_each(mShapes.begin(), mShapes.end(),
                  [x, y, angle](std::unique_ptr<ITransformable>& object) {
        object->rotate(x, y, angle);
    });
}

void Canvas::scale(double x, double y, double factor) {
    std::for_each(mShapes.begin(), mShapes.end(),
                  [x, y, factor](std::unique_ptr<ITransformable>& object) {
        object->scale(x, y, factor);
    });
}

void Canvas::draw(std::unique_ptr<ITransformable> shape) {
    mShapes.push_back(std::move(shape));
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator Canvas::begin() const {
    return mShapes.begin();
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator Canvas::end() const {
    return mShapes.end();
}

// End: Canvas

// Start: mShapes

Point::Point(double x, double y) : mX(x), mY(y) {}

double Point::x() const {
    return mX;
}

double Point::y() const {
    return mY;
}

void Point::translate(double x, double y) {
    mX += x; mY += y;
}

void Point::rotate(double x, double y, double angle) {
    // translate to center
    mX -= x; mY -= y;

    // rotate
    double s = sin(degree_to_radian(angle));
    double c = cos(degree_to_radian(angle));
    double newX = mX * c - mY * s;
    double newY = mX * s + mY * c;

    // translate back
    mX = newX + x; mY = newY + y;
}

void Point::scale(double x, double y, double factor) {
    // translate to center
    mX -= x; mY -= y;

    // scale
    double newX = mX * factor;
    double newY = mY * factor;

    // translate back
    mX = newX + x; mY = newY + y;
}

Line::Line(double x1, double y1, double x2, double y2)
    : endpoints{std::make_unique<Point>(x1, y1), std::make_unique<Point>(x2, y2)} {}

double Line::ax() const {
    return endpoints[0]->x();
}

double Line::ay() const {
    return endpoints[0]->y();
}

double Line::bx() const {
    return endpoints[1]->x();
}

double Line::by() const {
    return endpoints[1]->y();
}

void Line::translate(double x, double y) {
    std::for_each(endpoints.begin(), endpoints.end(),
                  [x, y](std::unique_ptr<Point>& p) {
        p->translate(x, y);
    });
}

void Line::rotate(double x, double y, double angle) {
    std::for_each(endpoints.begin(), endpoints.end(),
                  [x, y, angle](std::unique_ptr<Point>& p) {
        p->rotate(x, y, angle);
    });
}

void Line::scale(double x, double y, double factor) {
    std::for_each(endpoints.begin(), endpoints.end(),
                  [x, y, factor](std::unique_ptr<Point>& p) {
        p->scale(x, y, factor);
    });
}

Circle::Circle(double x, double y, double radius)
    : center(std::make_unique<Point>(x, y)), mRadius(radius) {}

double Circle::x() const {
    return center->x();
}

double Circle::y() const {
    return center->y();
}

double Circle::r() const {
    return mRadius;
}

void Circle::translate(double x, double y) {
    center->translate(x, y);
}

void Circle::rotate(double x, double y, double angle) {
    center->rotate(x, y, angle);
}

void Circle::scale(double x, double y, double factor) {
    center->scale(x, y, factor);
    mRadius *= factor;
}

Rectangle::Rectangle(double x, double y, double w, double h) : mSides {
    std::make_unique<Line>(x, y, x, y + h), // A(x,y) - B(x, y + h)
    std::make_unique<Line>(x, y, x + w, y), // A(x,y) - D(x + w, y)
    std::make_unique<Line>(x + w, y, x + w, y + h), // D(x + w, y) - C(x + w, y + h)
    std::make_unique<Line>(x, y + h, x + w, y + h), // B(x, y + h) - C(x + w, y + h)
} {}


std::array<std::unique_ptr<Line>, 4>::const_iterator Rectangle::begin() const {
    return mSides.begin();
}

std::array<std::unique_ptr<Line>, 4>::const_iterator Rectangle::end() const {
    return mSides.end();
}

void Rectangle::translate(double x, double y) {
    std::for_each(mSides.begin(), mSides.end(), [x, y](std::unique_ptr<Line>& side) {
       side->translate(x, y);
    });
}

void Rectangle::rotate(double x, double y, double angle) {
    std::for_each(mSides.begin(), mSides.end(), [x, y, angle](std::unique_ptr<Line>& side) {
        side->rotate(x, y, angle);
    });
}

void Rectangle::scale(double x, double y, double factor) {
    std::for_each(mSides.begin(), mSides.end(), [x, y, factor](std::unique_ptr<Line>& side) {
        side->scale(x, y, factor);
    });
}

// End: mShapes