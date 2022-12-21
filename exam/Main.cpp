#include <vector>
#include <ranges>
#include <iostream>
#include "FileManip.hpp"

using Human = std::tuple<int, std::string, int, double, double, Point2D>;

int main(int argc, char** argv) {
    FileReader reader("in.txt");

    auto a = reader.read<int>();
    auto b = reader.read<double>();
    auto c = reader.read<std::string>();
    auto d = reader.read<Point2D>();

//    auto [e, f ,g ,h] = reader.read<int, int, double, std::string>();

    FileWriter writer("out.txt");

    writer.write<int>(16);
    writer.write<double>(999);
    writer.write<std::string>("hahaha");

    writer.write<int, int, double, Point2D, std::string>(4,45, 5.0, Point2D(), "hello");

    std::vector<Human> humans;
//    auto view = humans | std::views::filter [&humans](Human& h) {

//    }

    return EXIT_SUCCESS;
}