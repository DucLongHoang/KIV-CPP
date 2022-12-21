#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>
#include <string>
#include <utility>
#include <fstream>

class Point2D {
    double x, y;

    // format in file as 'x:y'
    friend std::istream& operator >> (std::istream& stream, Point2D& point) {
        std::string line;
        stream >> line;
        auto pos = line.find_first_of(':');
        point.x = std::stoi(line.substr(0, pos));
        point.y = std::stoi(line.substr(pos + 1));
        return stream;
    }

    // print out as 'x:y'
    friend std::ostream& operator << (std::ostream& stream, const Point2D& point) {
        stream << point.x << ':' << point.y;
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

        template<typename T1, typename ... T>
        auto read(T ... args) {
            T1 t1;
            mIfStream >> t1;
            read(t1);
            return std::tuple{t1, read(args ...)};
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

        template<typename T1, typename ... T>
        void write(T1 t1, T ... args) {
            write(t1);
            write(args...);
        }
};