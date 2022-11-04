#include <iostream>
#include <memory>
#include "DataLoader.h"
#include "Drawing.h"

class OptParser {

};

//dsdsd8
int main(int argc, char** argv) {
//    Canvas canvas;
//    std::unique_ptr<ITransformable> line = std::make_unique<Line>(5,6,10,2);
//    std::unique_ptr<ITransformable> circ = std::make_unique<Circle>(9,8,5);
//    std::unique_ptr<ITransformable> rect = std::make_unique<Rectangle>(5,5,5,5);
//
//    canvas.draw(std::move(line));
//    canvas.draw(std::move(circ));
//    canvas.draw(std::move(rect));
//
//    canvas.translate(-20, 40);


    DataLoader loader;
    std::unique_ptr<IDrawing> drawing = std::make_unique<SVGDrawing>();

    // nacteni vstupu
    if (!loader.load_from_file("vstup.txt")) {
        std::cerr << "Nepodarilo se nacist soubor" << std::endl;
        return -1;
    }

    // get left upper corner and right lower corner



    // init drawing - pass from options args
    drawing->init(50, 50);

    for (const auto& shape : loader) {
        switch (shape) {
            // mapping strings to enums
            const std::map<const std::string, const Input&> inputMap = {
                    { "line", Input::Line },
                    { "circle", Input::Circle },
                    { "rect", Input::Rectangle },
                    { "translate", Input::Translate },
                    { "rotate", Input::Rotate },
                    { "scale", Input::Scale }
            };
        }
    }

    drawing->save_to_file("someFileName");

    return EXIT_SUCCESS;
}