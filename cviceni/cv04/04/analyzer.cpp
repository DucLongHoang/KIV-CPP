#include "analyzer.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cmath>

CAnalyzer::CAnalyzer(std::vector<double>&& numbers)
	: mNumbers(numbers)
{
    std::ranges::sort(mNumbers);

    mDensity.reserve(mNumbers.size());
    mDistribution.reserve(mNumbers.size());
}

CAnalyzer CAnalyzer::Create(const std::string& fileName)
{
	std::vector<double> numbers;

	// otevre soubor
	std::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if (!ifs.is_open())
		throw std::invalid_argument{ "Nelze otevrit soubor " + fileName };

	// zjisti velikost a pole cisel zvetsi, aby se tam cisla vesla
	ifs.seekg(0, std::ios::end);
	numbers.resize(ifs.tellg() / sizeof(double));
	ifs.seekg(0, std::ios::beg);

	// precte obsah souboru
	ifs.read(reinterpret_cast<char*>(numbers.data()), numbers.size() * sizeof(double));

	// overi, ze se precetlo vsechno
	if (ifs.gcount() != numbers.size() * sizeof(double))
		throw std::runtime_error{ "Nelze precist cisla ze souboru " + fileName };

	return CAnalyzer(std::move(numbers));
}

void CAnalyzer::Calculate_Density()
{
	// TODO


}

void CAnalyzer::Calculate_Distribution()
{
    for (size_t x = 0; x != mNumbers.size(); ++x) {
        double Fx = (x + 1.0) / mNumbers.size();
        mDistribution.push_back(Fx);
    }
}

void CAnalyzer::Analyze_Distribution(double& mean, double& sigma) const noexcept
{
	// Mean
    double sum = std::accumulate(mNumbers.begin(), mNumbers.end(), 0.0);
    mean = sum / mNumbers.size();

    // Standard deviation
    std::function calc_magic = [mean](double accumulator, double x) -> double {
        return pow(x - mean, 2);
    };
    double numerator = std::accumulate(mNumbers.begin(), mNumbers.end(), 0.0, calc_magic);

    double fraction = numerator /  mNumbers.size();
    sigma = pow(fraction, 0.5);
}

double CAnalyzer::Get_Percentile(unsigned char pct) const
{
    double index = (pct / 100.0) * mNumbers.size();
	return mNumbers[round(index)];
}

size_t CAnalyzer::Get_Numbers_Above_Percentile(unsigned char pct) const
{
    double index = (pct / 100.0) * mNumbers.size();
	return mNumbers.size() - round(index);
}