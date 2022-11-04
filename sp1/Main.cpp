#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <functional>
#include "DataLoader.h"
#include "Drawing.h"

// enum class defining valid output files
enum class FileType {
    SVG, PGM
};

const std::map<const std::string, const FileType> typeMap{
        {"svg", FileType::SVG},
        {"pgm", FileType::PGM},
};

// unnecessary Functor to get a FileType
class GetFileType {
    public:
        FileType operator()(const std::string& str) const {
            auto itr = typeMap.find(str);
            if (itr == typeMap.end()) {
                std::cout << "Unrecognized file type: " << str << std::endl;
                std::cout << "Recognized types are:   ";

                for (const auto& pair : typeMap) {
                    std::cout << pair.first << ", ";
                }
                std::cout << std::endl;
                exit(EXIT_FAILURE);
            }
            return itr->second;
        }
};

class OptParser {
    private:
        static constexpr int OPT_COUNT = 4;
        static constexpr int IDX_INPUT = 1;
        static constexpr int IDX_OUTPUT = 2;
        static constexpr int IDX_DIM = 3;

    public:
        std::string mInputFile, mOutputFile, fileExt;
        unsigned int mWidth, mHeight;
        FileType fileType;

        OptParser(int argc, char** argv) {
            if (argc != OPT_COUNT) {
                std::cerr << "Necessary number of args:  " << OPT_COUNT << std::endl;
                std::cerr << "Found number of arguments: " << argc << std::endl;
                exit(EXIT_FAILURE);
            }
            // file to read from
            mInputFile = std::string{argv[IDX_INPUT]};

            // file to write into
            std::stringstream stream{argv[IDX_OUTPUT]};
            std::getline(stream, mOutputFile, '.');
            std::getline(stream, fileExt, '.');

            // using functor to get file type
            GetFileType gft;
            fileType = gft(fileExt);

            // get dimension of output file
            stream = std::stringstream{argv[IDX_DIM]};
            std::string holder;
            std::getline(stream, holder, 'x');
            mWidth = std::stoi(holder);
            std::getline(stream, holder, 'x');
            mHeight = std::stoi(holder);
        }
};

int main(int argc, char** argv) {
    OptParser options{argc, argv};
    auto loader = std::make_unique<DataLoader>();
    std::unique_ptr<IDrawing> drawing;

    switch (options.fileType) {
        case FileType::PGM:
            drawing = std::make_unique<PGMDrawing>();
            break;
        case FileType::SVG:
            drawing = std::make_unique<SVGDrawing>();
            break;
    }

    // reading input file
    if (!loader->load_from_file(options.mInputFile)) {
        std::cerr << "Could not read file: " << options.mInputFile << std::endl;
        exit(EXIT_FAILURE);
    }

    // init drawing - pass from options args
    drawing->init(options.mWidth, options.mHeight);

    // xsome really shitty code
    for (const auto& shape : *loader) {
        if (dynamic_cast<Line*>(shape.get()) != nullptr) {
            auto* l = dynamic_cast<Line*>(shape.get());
            drawing->draw_line(l->ax(), l->ay(), l->bx(), l->by());
        }
        else if (dynamic_cast<Circle*>(shape.get()) != nullptr) {
            auto* c = dynamic_cast<Circle*>(shape.get());
            drawing->draw_circ(c->x(), c->y(), c->r());
        }
        else if (dynamic_cast<Rectangle*>(shape.get()) != nullptr) {
            auto* r = dynamic_cast<Rectangle*>(shape.get());

            for (const auto& side : *r) {
                drawing->draw_line(side->ax(), side->ay(), side->bx(), side->by());
            }
        }
    }

    drawing->save_to_file(options.mOutputFile);

    std::cout << "OK" << std::endl;
    std::cout << loader->commandCount() << std::endl;
    return EXIT_SUCCESS;
}