#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>
#include <string>
#include <utility>
#include <fstream>

template<double M1, double M2>
concept Point = true;

class Point2D {
    double mX, mY;

    public:
        Point2D() = default;
        Point2D(double x, double y) : mX(x), mY(y) {}

        // format in file as 'x:y'
        friend std::istream& operator >> (std::istream& stream, Point2D& point) {
            std::string line;
            stream >> line;
            auto pos = line.find_first_of(':');
            point.mX = std::stoi(line.substr(0, pos));
            point.mY = std::stoi(line.substr(pos + 1));
            return stream;
        }

        // print out as 'mX:mY'
        friend std::ostream& operator << (std::ostream& stream, const Point2D& point) {
            stream << point.mX << ':' << point.mY;
            return stream;
        }
};

class FileReader {
    private:
        std::ifstream mIfStream;

    public:
        explicit FileReader(const std::string& filename) {
            mIfStream = std::ifstream(filename);
        }

        template<typename T>
        T read() {
            T t;
            mIfStream >> t;
            return t;
        }

        template<typename ... Ts>
        auto read_many() {
            return std::make_tuple(read<Ts>() ...);
        }
};

class FileWriter {
    private:
        std::ofstream mOfStream;

    public:
        explicit FileWriter(const std::string& filename) {
            mOfStream = std::ofstream (filename);
        }

        template<typename T>
        void write(T t) {
            mOfStream << t << std::endl;
        }

        template<typename ... Ts>
        void write(Ts ... ts) {
            ( (mOfStream << ts << std::endl), ... );
        }
};