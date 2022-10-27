#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "Transformable.h"

class DataLoader final {
    public:

        enum class Input {
            Line, Circle, Rectangle, Translate, Rotate, Scale, Error
        };

        // mapping strings to enums
        const std::map<const std::string, Input> inputMap = {
                { "line", Input::Line },
                { "circle", Input::Circle },
                { "rect", Input::Rectangle },
                { "translate", Input::Translate },
                { "rotate", Input::Rotate },
                { "scale", Input::Scale }
        };

        DataLoader() = default;
        bool load_from_file(const std::string& file);
        void handle_line(const std::string& fileLine);
        void process_input(const std::vector<std::string>& data);

        std::vector<std::unique_ptr<ITransformable>>::const_iterator begin() const;
        std::vector<std::unique_ptr<ITransformable>>::const_iterator end() const;

    private:
        size_t mCommandCounter;
        std::unique_ptr<Canvas> mCanvas;
};
