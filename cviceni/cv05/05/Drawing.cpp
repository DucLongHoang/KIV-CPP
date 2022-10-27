#include "Drawing.h"
#include <sstream>
#include <fstream>

/******************************************** SVG *************************************************/

/*

Priklad SVG:
 
<svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
    <line x1="0" y1="80" x2="100" y2="20" stroke="black" />
</svg>

*/

void SVGDrawing::Init(int width, int height)
{
    std::ostringstream output("<svg viewBox=\"", std::ios::app);
    output << 0 << " " << 0 << " " << width << " " << height << "\" ";
    output << "xmlns=\"http://www.w3.org/2000/svg\">";

    mCommands.push_back(output.str());
}

std::ostream& operator<<(std::ostream& os, IDrawing::Color& c) {
    switch (c) {
        case IDrawing::Color::Black: {
            os << "black";
            break;
        }
        case IDrawing::Color::Blue: {
            os << "blue";
            break;
        }
        case IDrawing::Color::Green: {
            os << "green";
            break;
        }
        case IDrawing::Color::Yellow: {
            os << "yellow";
            break;
        }
        case IDrawing::Color::Red: {
            os << "red";
            break;
        }
    }
    return os;
}

void SVGDrawing::Draw_Line(int fromX, int fromY, int toX, int toY, Color clr)
{
    std::ostringstream output("<line ", std::ios::app);
    output << "x1=\"" << fromX << "\" " ;
    output << "y1=\"" << fromY << "\" " ;
    output << "x2=\"" << toX << "\" " ;
    output << "y2=\"" << toY << "\" " ;
    output << "stroke=\"" << clr << "\" />" ;

    mCommands.push_back(output.str());
}

void SVGDrawing::Save_To_File(const std::string& fileBaseName)
{
    //
    mCommands.push_back("</svg>");
    std::ostringstream outputFile(fileBaseName, std::ios::app);
    outputFile << ".svg";

    std::ofstream file(outputFile.str(), std::ios::out);

    std::for_each(mCommands.begin(), mCommands.end(), [&file](const std::string& command) {
       file << command << std::endl;
    });

    file.close();
}


/******************************************** PPM *************************************************/

/*

Priklad PPM:

P6
100 100
255
<pole bajtu>



pole bajtu je posloupnost binarnich R, G a B slozek za sebou, pixely jdou zleva doprava, shora dolu

*/

void PPMDrawing::Init(int width, int height)
{
    //
}

void PPMDrawing::Draw_Line(int fromX, int fromY, int toX, int toY, Color clr)
{
    //
}

void PPMDrawing::Save_To_File(const std::string& fileBaseName)
{
    //
}
