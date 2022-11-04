#pragma once

#include <vector>
#include <array>
#include <memory>

class ITransformable {
    public:
        virtual void translate(double x, double y) = 0;
        virtual void rotate(double x, double y, double angle) = 0;
        virtual void scale(double x, double y, double factor) = 0;
};

class Canvas : public ITransformable {
    public:
        Canvas() = default;

        virtual void translate(double x, double y) override;
        virtual void rotate(double x, double y, double angle) override;
        virtual void scale(double x, double y, double factor) override;
        virtual void draw(std::unique_ptr<ITransformable> shape);

        std::vector<std::unique_ptr<ITransformable>>::const_iterator begin() const;
        std::vector<std::unique_ptr<ITransformable>>::const_iterator end() const;
        ~Canvas() = default;

    private:
        std::vector<std::unique_ptr<ITransformable>> mShapes;
};

class Point : public ITransformable {
    public:
        Point(double x, double y);

        virtual void translate(double x, double y) override;
        virtual void rotate(double x, double y, double angle) override;
        virtual void scale(double x, double y, double factor) override;
        double x() const;
        double y() const;

    private:
        double mX, mY;
};

class Line : public ITransformable {
    public:
        Line(double x1, double y1, double x2, double y2);

        virtual void translate(double x, double y) override;
        virtual void rotate(double x, double y, double angle) override;
        virtual void scale(double x, double y, double factor) override;
        double ax() const;
        double ay() const;
        double bx() const;
        double by() const;

    private:
        std::array<std::unique_ptr<Point>, 2> endpoints;

};

class Circle : public ITransformable {
    public:
        Circle(double x, double y, double radius);

        virtual void translate(double x, double y) override;
        virtual void rotate(double x, double y, double angle) override;
        virtual void scale(double x, double y, double factor) override;
        double x() const;
        double y() const;
        double r() const;

    private:
        std::unique_ptr<Point> center;
        double mRadius;
};

class Rectangle : public ITransformable {
    public:
        Rectangle(double x, double y, double w, double h);

        virtual void translate(double x, double y) override;
        virtual void rotate(double x, double y, double angle) override;
        virtual void scale(double x, double y, double factor) override;
        std::array<std::unique_ptr<Line>, 4>::const_iterator begin() const;
        std::array<std::unique_ptr<Line>, 4>::const_iterator end() const;

    private:
        std::array<std::unique_ptr<Line>, 4> mSides;
};