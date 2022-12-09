#pragma once

constexpr double NumStart = -1.0;
constexpr double NumEnd = 1.0;
constexpr double NumRange = NumEnd - NumStart;
constexpr size_t NumCount = 20'000'000;

constexpr size_t ImgWidth = 100;
constexpr size_t ImgHeight = 100;

constexpr double IntervalSizeX = NumRange / static_cast<double>(ImgWidth);
constexpr double IntervalSizeY = NumRange / static_cast<double>(ImgHeight);

constexpr const char* InFileName = "input.bin";
