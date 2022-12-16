#include <iostream>
#include <unordered_map>
#include "mp_terminal.hpp"

// accepted modes
static const std::regex REGEX_MODES("[1-3]{1}");
// index of mode num
static constexpr int IDX_MODE = 1;

/**
 * Available modes
 * UNLIMITED -> unlimited precision
 * INTEGER   -> 32-bit precision
 * SHOWCASE  -> some demo
 */
enum class Mode {
    UNLIMITED, INTEGER, SHOWCASE
};

// map of modes
const std::unordered_map<int, Mode> modeMap = {
        {1, Mode::UNLIMITED},
        {2, Mode::INTEGER},
        {3, Mode::SHOWCASE},
};

/**
 * Class OptParser - consists of static methods and members
 *
 * Always first check args and then access Mode
 */
class OptParser {
    private:
        static constexpr int ARGS_COUNT = 2;

    public:
        static Mode MODE;

        static bool check_args(int argc, char** argv) {
            if (argc != ARGS_COUNT) return false;

            std::string str{argv[IDX_MODE]};
            if (!std::regex_match(str, REGEX_MODES)) return false;

            auto magic = [](int mode) -> bool {
                if (modeMap.contains(mode)) {
                    MODE = modeMap.find(mode)->second;
                    return true;
                }
                return false;
            };
            return magic(std::stoi(str));
        }
};
Mode OptParser::MODE = Mode::UNLIMITED;


int main(int argc, char** argv) {
    if (!OptParser::check_args(argc, argv)) {
        std::cout << "Incorrect arguments :(" << std::endl;
        exit(-1);
    }
    switch (OptParser::MODE) {
        case Mode::UNLIMITED: {
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|                    MPCalc - unlimited mode                    |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|Input a simple arithmetic expression with at most one operation|" << std::endl;
            std::cout << "|                 Binary operations: +, -, *, /                 |" << std::endl;
            std::cout << "|                      Unary operations: !                      |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;

            MPTerm<MPI_UNLIMITED> shell;
            shell.run(std::cin);
            break;
        }
        case Mode::INTEGER: {
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|                 MPCalc - limited mode (32-bit)                |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|Input a simple arithmetic expression with at most one operation|" << std::endl;
            std::cout << "|                 Binary operations: +, -, *, /                 |" << std::endl;
            std::cout << "|                      Unary operations: !                      |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;

            MPTerm<4> shell;
            shell.run(std::cin);
            break;
        }
        case Mode::SHOWCASE: {
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|                    MPCalc - showcase mode                     |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;
            std::cout << "|                      Watch magic happen!                      |" << std::endl;
            std::cout << "|***************************************************************|" << std::endl;

            MPTerm<MPI_UNLIMITED> shell;
            std::istringstream iss{
                "99999999*9999999999 "
                "1+$2 "
                "bank "
                "1-$1 "
                "69696969-420420420 "
                "$1*$2 "
                "bank "
                "100! "
                "1-$1 "
                "$1*$4 "
                "15674893/36545 "
                "$1/0 "
                "exit"};

            shell.run(iss);
            break;
        }
    }
    return EXIT_SUCCESS;
}