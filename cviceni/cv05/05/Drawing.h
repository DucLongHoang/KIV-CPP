#pragma once

#include <string>
#include <vector>

class IDrawing
{
	public:
		enum class Color
		{
			Black,
			Blue,
			Green,
			Yellow,
			Red
		};

		// inicializace vykreslovace, platna apod.
		virtual void Init(int width, int height) = 0;
		// vykresli caru z [fromX, fromY] do [toX, toY] barvou 'clr'
		virtual void Draw_Line(int fromX, int fromY, int toX, int toY, Color clr) = 0;
		// ulozi vystup do souboru, fileBaseName je nazev souboru bez pripony; priponu pridava az implementace
		virtual void Save_To_File(const std::string& fileBaseName) = 0;
};

class SVGDrawing : public IDrawing
{
	private:
		//
        std::vector<std::string> mCommands;

	public:
		virtual void Init(int width, int height) override;
		virtual void Draw_Line(int fromX, int fromY, int toX, int toY, Color clr) override;
		virtual void Save_To_File(const std::string& fileBaseName) override;
};

class PPMDrawing : public IDrawing
{
	private:
		//

	public:
		virtual void Init(int width, int height) override;
		virtual void Draw_Line(int fromX, int fromY, int toX, int toY, Color clr) override;
		virtual void Save_To_File(const std::string& fileBaseName) override;
};
