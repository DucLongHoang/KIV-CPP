#include "DataLoader.h"
#include <fstream>
#include <sstream>

bool Data_Loader::Load(const std::string& file)
{

	// Hlavicka: x1,y1,x2,y2,color
	// cisla jsou integery, color je retezec (konkretne jeden z vyctu: black, blue, green, yellow, red)
    std::ifstream inputFile(file);
    std::string fileLine;

    std::getline(inputFile, fileLine);
    if (fileLine != "x1,y1,x2,y2,color") return false;

    while (std::getline(inputFile, fileLine)) {
    std::istringstream lineStreamed(fileLine);
        std::string token;
        Line lineToDraw;

        std::getline(lineStreamed, token, ',');
        lineToDraw.x1 = std::stoi(token);
        std::getline(lineStreamed, token, ',');
        lineToDraw.y1 = std::stoi(token);
        std::getline(lineStreamed, token, ',');
        lineToDraw.x2 = std::stoi(token);
        std::getline(lineStreamed, token, ',');
        lineToDraw.y2 = std::stoi(token);
        std::getline(lineStreamed, token, ',');
        lineToDraw.color = token;

        mLines.push_back(lineToDraw);
    }
	return true;
}

std::list<Data_Loader::Line>::const_iterator Data_Loader::begin() const
{
	return mLines.begin();
}

std::list<Data_Loader::Line>::const_iterator Data_Loader::end() const
{
	return mLines.end();
}
