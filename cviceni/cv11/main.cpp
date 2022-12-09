#include <iostream>
#include <fstream>
#include <vector>

#include "drawing.hpp"

#include <filesystem>

#include "generator.hpp"
#include "analyzer.hpp"
#include "shared.hpp"

#include <chrono>
#include <memory>

int main(int argc, char** argv)
{
    bool dummy = false;
    if (argc > 1 && std::string_view(argv[1]) == "dummy")
        dummy = true;

    std::cout << "Analyzator cisel v souboru (nebo neco takoveho)" << std::endl;
    std::cout << std::endl;

    if (!std::filesystem::exists(InFileName))
    {
        std::cout << "Vstupni soubor neexistuje, generuji..." << std::endl;

        Generator g;
        g.Set_Mean(0.0);
        g.Set_Sigma(0.2);
        g.Set_Number_Count(NumCount);
        g.Set_Limits(NumStart, NumEnd);

        g.Generate();

        std::cout << "Hotovo. Restartujte program." << std::endl;
    }
    else
    {
        std::cout << "Vstupni soubor existuje, analyzuji..." << std::endl;

        auto start_time = std::chrono::steady_clock::now();

        std::unique_ptr<IAnalyzer> a;
        if (dummy)
            a = std::make_unique<DummyAnalyzer>();
        else
            a = std::make_unique<Dist2DAnalyzer>();
        a->Load();

        a->Analyze();

        a->Save_Image("distribution");

        auto end_time = std::chrono::steady_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        std::cout << "Hotovo" << std::endl;
        std::cout << "Celkovy cas na analyzu " << (a->Get_File_Size() / (1024 * 1024)) << " MiB dat a vygenerovani obrazku " << ImgWidth << "x" << ImgHeight << ": " << dur << "ms" << std::endl;

        a.reset();
    }

    return 0;
}
