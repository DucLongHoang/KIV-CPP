#include "analyzer.hpp"

#include <fstream>
#include "shared.hpp"
#include "drawing.hpp"

bool Dist2DAnalyzer::Load()
{
    std::ifstream ifs(InFileName, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        return false;

    ifs.seekg(0, std::ios::end);
    mFile_Size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    mData = new double[mFile_Size / sizeof(double)];
    size_t i = 0;
    while (ifs)
        ifs.read((char*)&mData[i++], sizeof(double));

    return true;
}

bool Dist2DAnalyzer::Analyze()
{
    for (size_t i = 0; i < mFile_Size / sizeof(double) - 1; i += 2)
    {
        double d1 = mData[i];
        double d2 = mData[i+1];

        size_t x = static_cast<size_t>((d1 - NumStart) / IntervalSizeX);
        size_t y = static_cast<size_t>((d2 - NumStart) / IntervalSizeY);

        try
        {
            Histogram.at(x).at(y)++;
        }
        catch (...)
        {
            if (x < ImgWidth && y < ImgHeight)
            {
                while (x >= Histogram.size())
                    Histogram.push_back(std::vector<size_t>());

                while (y >= Histogram[x].size())
                    Histogram[x].push_back(0);
            }
        }
    }

    mMaximum = 0;
    for (size_t i = 0; i < Histogram.size(); i++)
    {
        for (size_t j = 0; j < Histogram[i].size(); j++)
        {
            if (Histogram[i][j] > mMaximum)
                mMaximum = Histogram[i][j];
        }
    }

    return true;
}

bool Dist2DAnalyzer::Save_Image(const std::string& out)
{
    PPMDrawing draw;
    draw.Init(ImgWidth, ImgHeight, out);
    for (size_t i = 0; i < Histogram.size(); i++)
    {
        for (size_t j = 0; j < Histogram[i].size(); j++)
        {
            double factor = 1.0 - (static_cast<double>(Histogram[i][j]) / static_cast<double>(mMaximum));

            uint8_t grayscale = static_cast<uint8_t>(255.0 * factor);

            IDrawing::Color c{ grayscale, grayscale, grayscale };

            draw.Draw_Pixel(static_cast<int>(i), static_cast<int>(j), c);
        }
    }
    draw.Save();

    return true;
}