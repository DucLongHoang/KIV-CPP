#include <algorithm>
#include "Transformable.h"

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
    mShapes.emplace_back(std::move(shape));
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator Canvas::begin() const {
    return mShapes.begin();
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator Canvas::end() const {
    return mShapes.end();
}

// End: Canvas

// Start: mShapes

Line::Line(double x1, double y1, double x2, double y2)
    : mX1(x1), mY1(y1), mX2(x2), mY2(y2) {}

void Line::translate(double x, double y) {
    mX1 += x; mX2 += x;
    mY1 += y; mY2 += y;
}

void Line::rotate(double x, double y, double angle) {

}

void Line::scale(double x, double y, double factor) {

}

Circle::Circle(double x, double y, double radius)
    : mX(x), mY(y), mRadius(radius) {}

void Circle::translate(double x, double y) {
    mX += x; mY += y;
}

void Circle::rotate(double x, double y, double angle) {

}

void Circle::scale(double x, double y, double factor) {

}

Rectangle::Rectangle(double x, double y, double w, double h) : sides {
    std::make_unique<Line>(x, y, x, y + h), // A(x,y) - B(x, y + h)
    std::make_unique<Line>(x, y, x + w, y), // A(x,y) - D(x + w, y)
    std::make_unique<Line>(x + w, y, x + w, y + h), // D(x + w, y) - C(x + w, y + h)
    std::make_unique<Line>(x, y + h, x + w, y + h), // B(x, y + h) - C(x + w, y + h)
} {}

void Rectangle::translate(double x, double y) {
    std::for_each(sides.begin(), sides.end(), [x, y](std::unique_ptr<Line>& side) {
       side->translate(x, y);
    });
}

void Rectangle::rotate(double x, double y, double angle) {
    std::for_each(sides.begin(), sides.end(), [x, y, angle](std::unique_ptr<Line>& side) {
        side->rotate(x, y, angle);
    });
}

void Rectangle::scale(double x, double y, double factor) {
    std::for_each(sides.begin(), sides.end(), [x, y, factor](std::unique_ptr<Line>& side) {
        side->scale(x, y, factor);
    });
}

// End: mShapes