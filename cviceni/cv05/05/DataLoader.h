#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <list>

class Data_Loader final
{
	public:
		// struktura cary
		struct Line
		{
			int x1, y1, x2, y2;
			std::string color;
		};

	public:
		Data_Loader() = default;

		// nacte CSV soubor ze zadane cesty
		bool Load(const std::string& file);

		// transparentni begin() pro vnitrni seznam car
		std::list<Line>::const_iterator begin() const;
		// transparentni end() pro vnitrni seznam car
		std::list<Line>::const_iterator end() const;

	private:
		// seznam car ze souboru
		std::list<Line> mLines;
};
