#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "Drawing.h"

void SVGDrawing::init(size_t width, size_t height) {
    mOutputStream = std::ostringstream("<svg viewBox=\"", std::ios::app);
    mOutputStream << 0 << " " << 0 << " " << width << " " << height << "\" ";
    mOutputStream << "xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
}

void SVGDrawing::draw_line(double x1, double y1, double x2, double y2) {
    mOutputStream << "<line ";
    mOutputStream << "x1=\"" << x1 << "\" ";
    mOutputStream << "y1=\"" << y1 << "\" ";
    mOutputStream << "x2=\"" << x2 << "\" ";
    mOutputStream << "y2=\"" << y2 << "\" ";
    mOutputStream << "stroke=\"black\" />" << std::endl;
}

void SVGDrawing::draw_circ(double x, double y, double r) {
    mOutputStream << "<circle ";
    mOutputStream << "cx=\"" << x << "\" ";
    mOutputStream << "cy=\"" << y << "\" ";
    mOutputStream << "r=\"" << r << "\" ";
    mOutputStream << "stroke=\"black\" fill=\"none\" />" << std::endl;
}

void SVGDrawing::draw_rect(double x, double y, double w, double h) {
    mOutputStream << "<rect ";
    mOutputStream << "x=\"" << x << "\" ";
    mOutputStream << "y=\"" << y << "\" ";
    mOutputStream << "width=\"" << w << "\" ";
    mOutputStream << "height=\"" << h << "\" ";
    mOutputStream << "stroke=\"black\" fill=\"none\" />" << std::endl;
}

void SVGDrawing::save_to_file(const std::string &fileBaseName) {
    std::ostringstream outputFile(fileBaseName, std::ios::app);
    outputFile << ".svg";

    std::ofstream file(outputFile.str(), std::ios::out);
    mOutputStream << "</svg>" << std::endl;
    file << mOutputStream.str() << std::endl;
}

void PGMDrawing::init(size_t width, size_t height) {
    mMatrix.resize(height);
    std::for_each(mMatrix.begin(), mMatrix.end(),
                  [width](std::vector<int>& row){
        row.resize(width, 0);
    });

    mOutputStream = std::ostringstream("P2", std::ios::app);
    mOutputStream << std::endl;
    mOutputStream << "#" << " comment line" << std::endl;
    mOutputStream << width << " " << height << std::endl;
}

void PGMDrawing::draw_line(double x1, double y1, double x2, double y2) {
    // casting doubles to int
    x1 = (int)x1; y1 = (int)y1; x2 = (int)x2; y2 = (int)y2;

    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while(true){  /* loop */
        set_pixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; } /* e_xy + e_x > 0 */
        if (e2 <= dx) { err += dx; y1 += sy; } /* e_xy + e_y < 0 */
    }
}

void PGMDrawing::draw_circ(double cx, double cy, double r) {
    // casting doubles to int
    cx = (int)cx; cy = (int)cy; r = (int)r;

    int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */
    do {
        set_pixel(cx - x, cy + y); /*   I. Quadrant */
        set_pixel(cx - y, cy - x); /*  II. Quadrant */
        set_pixel(cx + x, cy - y); /* III. Quadrant */
        set_pixel(cx + y, cy + x); /*  IV. Quadrant */
        r = err;
        if (r <= y) err += ++y * 2 + 1;           /* e_xy+e_y < 0 */
        if (r > x || err > y) err += ++x * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (x < 0);
}

void PGMDrawing::draw_rect(double x, double y, double w, double h) {

}

void PGMDrawing::save_to_file(const std::string &fileBaseName) {
    std::ostringstream outputFile(fileBaseName, std::ios::app);
    outputFile << ".pgm";

    for (const auto& row : mMatrix) {
        for (const auto& cell : row) {
            mOutputStream << cell << " ";
        }
        mOutputStream << std::endl;
    }

    std::ofstream file(outputFile.str(), std::ios::out);
    file << mOutputStream.str() << std::endl;
}

void PGMDrawing::set_pixel(size_t x, size_t y) {
    // xy coordinates are inverted in a matrix
    mMatrix[y][x] = 1;
}