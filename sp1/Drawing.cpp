#include <fstream>
#include "Drawing.h"

void SVGDrawing::init(int width, int height) {
    mOutputStream = std::ostringstream("<svg viewBox=\"", std::ios::app);
    mOutputStream << 0 << " " << 0 << " " << width << height << "\" ";
    mOutputStream << "xmlns=\"http://www.w3.org/2000/svg\">";
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
    file << mOutputStream.str() << std::endl;
}

void PGMDrawing::init(int width, int height) {

}

void PGMDrawing::draw_line(double x1, double y1, double x2, double y2) {

}

void PGMDrawing::draw_circ(double x, double y, double r) {

}

void PGMDrawing::draw_rect(double x, double y, double w, double h) {

}

void PGMDrawing::save_to_file(const std::string &fileBaseName) {
    std::ostringstream outputFile(fileBaseName, std::ios::app);
    outputFile << ".pgm";

    std::ofstream file(outputFile.str(), std::ios::out);
    file << mOutputStream.str() << std::endl;
}