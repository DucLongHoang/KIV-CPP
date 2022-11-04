#include <sstream>
#include <ranges>
#include <string_view>
#include "DataLoader.h"

DataLoader::DataLoader() : mCommandCounter{0}, mCanvas{std::make_unique<Canvas>()} {}

bool DataLoader::load_from_file(const std::string &file) {
    std::ifstream inputFile(file);
    std::string fileLine;

    while (std::getline(inputFile, fileLine)) {
//        std::cout << "i am inside" << std::endl;
        handle_line(fileLine);
    }
    return true;
}

void DataLoader::handle_line(const std::string &fileLine) {
    std::vector<std::string> data;
    std::stringstream lineStream;

    // removing comments
    size_t commentIndex = fileLine.find('#');
    if (commentIndex == std::string::npos)
        lineStream = std::stringstream(fileLine);
    else
        lineStream = std::stringstream(fileLine.substr(0, commentIndex));

    std::string token;
    while (std::getline(lineStream, token, ' ')) {
        data.push_back(token);
    }
    process_input(data);
}

void DataLoader::process_input(const std::vector<std::string>& data) {
    if (data.empty()) return;

    // lambda for finding mapping of input
    auto mapInput = [this](const std::string& str) -> DataLoader::Input {
        auto itr = DataLoader::inputMap.find(str);
        if (itr == DataLoader::inputMap.end())
            return DataLoader::Input::Error;
        return itr->second;
    };

    auto inputType = mapInput(data[0]);

    auto doubleValues = data
            | std::views::drop(1)
            | std::views::transform([](const std::string& s) -> double {
                return std::stof(s);
            });

    switch (mapInput(data[0])) {
        case Input::Line: {
            mCanvas->draw(std::make_unique<Line>(doubleValues[0], doubleValues[1], doubleValues[2], doubleValues[3]));
            break;
        }
        case Input::Circle: {
            mCanvas->draw(std::make_unique<Circle>(doubleValues[0], doubleValues[1], doubleValues[2]));
            break;
        }
        case Input::Rectangle: {
            mCanvas->draw(std::make_unique<Rectangle>(doubleValues[0], doubleValues[1], doubleValues[2], doubleValues[3]));
            break;
        }
        case Input::Translate: {
            mCanvas->translate(doubleValues[0], doubleValues[1]);
            break;
        }
        case Input::Rotate: {
            mCanvas->rotate(doubleValues[0], doubleValues[1], doubleValues[2]);
            break;
        }
        case Input::Scale: {
            mCanvas->scale(doubleValues[0], doubleValues[1], doubleValues[2]);
            break;
        }
        case Input::Error: {
            break;
        }
    }
    
    mCommandCounter++;
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator DataLoader::begin() const {
    return mCanvas->begin();
}

std::vector<std::unique_ptr<ITransformable>>::const_iterator DataLoader::end() const {
    return mCanvas->end();
}