#include "Transformable.h"

// Start: Canvas

Canvas::Canvas(size_t w, size_t h)
    : mW(w), mH(h) {}

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

Rectangle::Rectangle(double x, double y, double w, double h)
    : mX(x), mY(y), mW(w), mH(h) {}

void Rectangle::translate(double x, double y) {
    mX += x; mY += y;
}

void Rectangle::rotate(double x, double y, double angle) {

}

void Rectangle::scale(double x, double y, double factor) {

}

// End: mShapes