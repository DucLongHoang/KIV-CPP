#pragma once

#include <vector>
#include <string>
#include <ranges>
#include "cmath"

class CAnalyzer final
{
	// minimalni uvazovane cislo ze vstupu (zjednoduseni)
	static constexpr double Min_Number = -100.0;
	// maximalni uvazovane cislo ze vstupu (zjednoduseni)
	static constexpr double Max_Number = 100.0;

	protected:
		// konstruktor - nacte cisla ze souboru, muze vyhodit vyjimku
		CAnalyzer(std::vector<double>&& numbers);

	public:
		static CAnalyzer Create(const std::string& fileName);

		// spocte funkci hustoty (empirickou) a ulozi ji do atributu tridy
		void Calculate_Density();

		// spocte distribucni funkci (empirickou) a ulozi ji do atributu tridy
		void Calculate_Distribution();

		// analyzuje distribuci cisel, vrati stredni hodnotu a rozptyl
		void Analyze_Distribution(double& mean, double& sigma) const noexcept;

		// ziska percentil 'pct' (rozsah 1 - 99, pro zjednoduseni)
		double Get_Percentile(unsigned char pct) const;

		// ziska pocet cisel nad percentilem 'pct' (rozsah 1 - 99, pro zjednoduseni)
		size_t Get_Numbers_Above_Percentile(unsigned char pct) const;

	private:
		std::vector<double> mNumbers;

		// TODO: funkce hustoty
		std::vector<double> mDensity;

		// TODO: distribucni funkce (empiricka kumulativni, ECDF)
		std::vector<double> mDistribution;

		// TODO: co dalsiho potrebujete
};