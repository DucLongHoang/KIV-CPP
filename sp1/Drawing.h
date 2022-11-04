#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>

class IDrawing {
    public:
        virtual void init(size_t width, size_t height) = 0;
        virtual void draw_line(double x1, double y1, double x2, double y2) = 0;
        virtual void draw_circ(double x, double y, double r) = 0;
        virtual void draw_rect(double x, double y, double w, double h) = 0;
        virtual void save_to_file(const std::string& fileBaseName) = 0;
};

class SVGDrawing : public IDrawing {
    public:
        virtual void init(size_t width, size_t height) override;
        virtual void draw_line(double x1, double y1, double x2, double y2) override;
        virtual void draw_circ(double x, double y, double r) override;
        virtual void draw_rect(double x, double y, double w, double h) override;
        virtual void save_to_file(const std::string& fileBaseName) override;

    private:
        std::ostringstream mOutputStream;
};

class PGMDrawing : public IDrawing {
    public:
        virtual void init(size_t width, size_t height) override;
        virtual void draw_line(double x1, double y1, double x2, double y2) override;
        virtual void draw_circ(double x, double y, double r) override;
        virtual void draw_rect(double x, double y, double w, double h) override;
        virtual void save_to_file(const std::string& fileBaseName) override;
        virtual void set_pixel(size_t width, size_t height);

    private:
        std::ostringstream mOutputStream;
        std::vector<std::vector<int>> mMatrix;
};