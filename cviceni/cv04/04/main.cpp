#include <iostream>
#include <random>
#include <fstream>

#include "analyzer.h"

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Nacitam soubor..." << std::endl;

		CAnalyzer analyzer = CAnalyzer::Create("04_data.bin");

		std::cout << "Analyzuji funkci hustoty..." << std::endl;

		analyzer.Calculate_Density();

		std::cout << "Analyzuji distribucni funkci..." << std::endl;

		analyzer.Calculate_Distribution();

		std::cout << "Analyzuji vlastnosti..." << std::endl;

		double mean = 0, sigma = 0;
		analyzer.Analyze_Distribution(mean, sigma);

		std::cout << "Stredni hodnota = " << mean << std::endl
				  << "Rozptyl         = " << sigma << std::endl;

		double pct = analyzer.Get_Percentile(90);

		std::cout << "90. percentil   = " << pct << std::endl;

		size_t cnt = analyzer.Get_Numbers_Above_Percentile(90);

		std::cout << "Pocet cisel nad 90. percentilem: " << cnt << std::endl;
	}
	catch (const std::exception& err)
	{
		std::cerr << "Vyskytla se chyba: " << err.what() << std::endl;
		return 1;
	}

	return 0;
}
