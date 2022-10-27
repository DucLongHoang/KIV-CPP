#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <array>

// aliasujeme si typ handleru
using THandler = std::function<bool(const std::string&)>;

// mapa handleru prikazu
std::map<std::string, THandler> Handler_Table;

void fill_handlers()
{
	// hello prikaz
	Handler_Table["hello"] = [](const std::string& str) -> bool {
		std::cout << "Zdravim, " << str << std::endl;
		return true;
	};

	// beep prikaz
	Handler_Table["beep"] = [](const std::string& str) -> bool {
		std::cout << "beep, beep, I'm a " << str << std::endl;
		return true;
	};

	// pocitani samohlasek ve vstupu
	Handler_Table["samohlasky"] = [](const std::string& str) -> bool {
		const std::array<const char, 6> smhl = {'a', 'e', 'i', 'y', 'o', 'u'};

		size_t pocet = std::count_if(str.begin(), str.end(), [&smhl](const char i) { return std::find(smhl.begin(), smhl.end(), i) != smhl.end(); });

		std::cout << "Vstup " << str << " obsahuje " << pocet << " samohlasek" << std::endl;
		return true;
	};

	// ukoncovaci prikaz
	Handler_Table["exit"] = [](const std::string& str) -> bool {
		return false;
	};
}

int main()
{
	// naplnime tabulku handleru
	fill_handlers();

	std::string command, opcode, arg;
	bool ret = true;

	std::cout << "Ukazkovy shell" << std::endl;

	while (ret)
	{
		// prompt
		std::cout << std::endl << "root@home:~$ ";
		// cteni ze standardniho vstupu
		std::getline(std::cin, command);

		// najdeme mezeru
		size_t pos = command.find_first_of(' ');
		if (pos != std::string::npos)
		{
			// pokud je mezera, vyparsujeme operacni kod a parametr
			opcode = command.substr(0, pos);
			arg = command.substr(pos + 1);
		}
		else
		{
			// pokud ne, operacnim kodem je cely vstup a parametr je prazdny
			opcode = command;
			arg = "";
		}

		// zname vubec prikaz?
		if (Handler_Table.find(opcode) == Handler_Table.end())
		{
			std::cerr << "Prikaz " << opcode << " neznam" << std::endl;
			continue;
		}

		// zavolame funkci pro obsluhu
		ret = Handler_Table[opcode](arg);
	}

	return 0;
}
