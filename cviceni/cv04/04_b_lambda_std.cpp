#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

int main()
{
	std::vector<int> src;
	std::vector<int> dst_1;
	std::vector<int> dst_2;

	// vygenerovani 20 sudych cisel
	src.resize(20);
	std::generate(src.begin(), src.end(), [n = 1]() mutable {
		return n++ * 2;
	});

	// funkce ktera bude zatrizovat vstupni cisla
	// a) do dst_1 pokud je cislo zaroven delitelne tremi
	// b) do dst_2 pokud ne
	auto organize = [&dst_1, &dst_2](const int num) -> bool {
		if (num % 3 == 0)
		{
			dst_1.push_back(num);
			return true;
		}

		dst_2.push_back(num);
		return false;
	};

	for (const auto& num : src)
	{
		if (organize(num))
			std::cout << "Cislo " << num << " zarazeno do 1" << std::endl;
	}

	std::cin.get();

	return 0;
}
