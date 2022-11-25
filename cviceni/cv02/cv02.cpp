#include <iostream>
#include <vector>
#include <memory>
#include <string>

namespace myConstants {
    // files
    const int FILE_COUNT = 2;
    const std::string FILE_NAME_BASE = "file";
    const std::string FILE_EXT = ".txt";

    // file content
    const std::string CONTENT_TEMPLATE = "some content ";
    const int UNIQUE_CONTENT = 0;

    const int BUFFER_SIZE = 1024;
}

// 1) implementujte RAII obal nad souborem ze standardni knihovny C
class RAII_File
{
    private:
        std::string mPath;
        FILE* mFile;

	public:
		// constructor
        RAII_File(const std::string& filePath) : mPath{filePath}{
            std::cout << "Creating RAII File" << std::endl;
            mFile = fopen(filePath.c_str(), "r+");
        }


		// 2) doplnte metodu Read_Line
		std::string Read_Line()
		{
            // + 1 for the '\n' char
			char buffer[myConstants::BUFFER_SIZE + 1];
            if (fgets(buffer, myConstants::BUFFER_SIZE, mFile) != nullptr) {
                return std::string{buffer};
            }
            return "";
		}

		// 3) doplnte metodu Write_Line
		void Write_Line(const std::string& line)
		{
            fputs(line.c_str(), mFile);
		}

		// destructor
        ~RAII_File(){
            std::cout << "Closing RAII File" << std::endl;
            fclose(mFile);
        }
};

/**
 * Method fills [vector] with [FILE_COUNT] RAII_Files.
 * @param vector to be filled with RAII_Files.
 */
void fillVector(std::vector<std::unique_ptr<RAII_File>>& vector) {
    for (size_t i = 0; i != myConstants::FILE_COUNT; ++i) {
        std::string file_name = myConstants::FILE_NAME_BASE;
        file_name.append(std::to_string(i));
        file_name.append(myConstants::FILE_EXT);
        vector.push_back(std::make_unique<RAII_File>(file_name));
    }
}

// 4) implementujte metodu, ktera vygeneruje obsah do nekolika souboru
void Generate_Files()
{
	// a) vytvorte vektor souboru
    std::vector<std::unique_ptr<RAII_File>> raii_files;

	// b) vlozte do nej nekolik instanci RAII_File souboru, do kterych budeme zapisovat
    fillVector(raii_files);

	// c) vygenerujte nejaky obsah, ktery do souboru budete vkladat;
	//    - kazdy soubor by mel mit ruzny obsah
	//    - pristupujte ke vsem souborum v cyklu nad vektorem
    int i = myConstants::UNIQUE_CONTENT;
    for (auto& file : raii_files) {
        std::string line = myConstants::CONTENT_TEMPLATE;
        line += std::to_string(i);
        ++i;
        file->Write_Line(line);
    }
}

// 5) implementujte metodu, ktera overi, ze v souborech je skutecne ten obsah, ktery tam ma byt
bool Verify_Files()
{
	// a) vytvorte vektor souboru
    std::vector<std::unique_ptr<RAII_File>> raii_files;

	// b) vlozte do nej nekolik instanci RAII_File souboru, ze kterych budeme cist
    fillVector(raii_files);

	// c) overte, ze v souborech je to, co jste do nich v Generate_Files zapsali
	//    - vrate true, pokud ano, jinak vratte false
    int i = myConstants::UNIQUE_CONTENT;
    for (auto& file : raii_files) {
        std::string file_line = file->Read_Line();
        std::string check_line = myConstants::CONTENT_TEMPLATE;
        check_line += std::to_string(i);
        ++i;

        if (check_line != file_line) return false;
    }

	return true;
}

int main(int argc, char** argv)
{
	Generate_Files();

	const bool outcome = Verify_Files();

	if (outcome)
		std::cout << "Vse probehlo v poradku" << std::endl;
	else
		std::cerr << "Vyskytla se chyba!" << std::endl;

	return EXIT_SUCCESS;
}