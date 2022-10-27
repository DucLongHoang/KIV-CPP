#include <iostream>
#include <algorithm>
#include <memory>
#include <map>

#include "DataLoader.h"
#include "Drawing.h"

#undef min
#undef max

// mapovani retezcu na hodnoty vyctu
const std::map<const std::string, IDrawing::Color> ColorMap = {
	{ "black", IDrawing::Color::Black },
	{ "blue", IDrawing::Color::Blue },
	{ "green", IDrawing::Color::Green },
	{ "red", IDrawing::Color::Red },
	{ "yellow", IDrawing::Color::Yellow }
};

int main(int argc, char** argv) {
	Data_Loader ldr;

	std::unique_ptr<IDrawing> draw = std::make_unique<SVGDrawing>();

	// nacteni vstupu
	if (!ldr.Load("sample.csv"))
	{
		std::cerr << "Nepodarilo se nacist soubor" << std::endl;
		return -1;
	}

	// nalezeni leveho horniho a praveho spodniho rohu obrazku
	Data_Loader::Line minmax = *ldr.begin();
	std::for_each(ldr.begin(), ldr.end(), [&minmax](const Data_Loader::Line& line) {
		minmax.x1 = std::min(std::min(line.x1, line.x2), minmax.x1);
		minmax.x2 = std::max(std::max(line.x1, line.x2), minmax.x2);
		minmax.y1 = std::min(std::min(line.y1, line.y2), minmax.y1);
		minmax.y2 = std::max(std::max(line.y1, line.y2), minmax.y2);
	});

	// inicializace kreslitka
	draw->Init(minmax.x2 - minmax.x1, minmax.y2 - minmax.y1);

	// lambda pro nalezeni mapovani barvy
	auto mapColor = [](const std::string& colStr) -> IDrawing::Color {
		auto itr = ColorMap.find(colStr);
		if (itr == ColorMap.end())
			return IDrawing::Color::Black; // unknown
		return itr->second;
	};

	// vykresleni car
	for (auto& line : ldr)
		draw->Draw_Line(line.x1, line.y1, line.x2, line.y2, mapColor(line.color));

	// ulozeni vystupu
	draw->Save_To_File("output");

	return 0;
}
