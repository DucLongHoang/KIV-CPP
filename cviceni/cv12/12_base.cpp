#include <iostream>
#include <chrono>
#include <random>
#include <ranges>
#include <vector>
#include <array>
#include <algorithm>
#include <execution>

/*
 * Priklad k optimalizaci
 * ======================
 * Tento program UMYSLNE obsahuje neefektivni konstrukce, pouziti nevhodnych datovych struktur, kombinace ridicich
 * struktur, ktere nedavaji uplne smysl, nebo algoritmus, ktery nejakym zpusobem nemusi byt uplne ten nejlepsi.
 * 
 * Prakticka ukazka toho, jak se i jednoduchy ukol da zprasit.
 *
 * Pokuste se zoptimalizovat beh programu tak, abyste usetrili alespon 25 % casu. Efektivita je samozrejme tesne
 * vazana na hardware a architekturu, ale minimalne 25 % casu se da usporit na drtive vetsine prostredi.
 *
 * Regiony k optimalizaci jsou oznaceny komentarovymi bloky BEGIN a END
 * 
 * Hinty, co muzete zavest nebo zlepsit:
 * - const, constexpr, pristupy do STL, reference, paralelismus, optimalizace cyklu
 * - datove typy, organizace v pameti, sablony
 * - zjistit, zda je vubec potreba ukladat urcita data
 * - volitelne muzete pouzit profiler
 *
 * Nejprve si sem zapiste referencni cas z prvniho behu: 33371 ms       [ slow PC :((( ]
 * Sem pak zapiste cas, ktery jste obdrzeli po zefektivneni: 12844 ms   [ still slow PC :((( ]
 *
 * Improvement: 61 %
 */

//  BEGIN: Tady dle potreby muzete neco zmenit

// pocet vstupnich cisel
static constexpr unsigned int inputs_size = 1'000'000;      // changed type, added static and constexpr
// minimalni generovane cislo
static constexpr unsigned short minnum = 1;                 // changed type, added static and constexpr
// maximalni generovane cislo
static constexpr unsigned short maxnum = 1'000;             // changed type, added static and constexpr

// pocet prubehu experimentu
static constexpr unsigned char experiment_run_count = 5;    // changed type, added static and constexpr

// array size = 1001, index = divisor
std::array<unsigned int, maxnum + 1> divisors;

// proved experiment!
void Do_Experiment(const std::vector<int>& inputs)
{
    // vycistime pole delitelu
    divisors.fill(0);

    // parallel, skip division of 0 and 1
    auto view_divisors = std::views::iota(2, 1001);
    std::for_each(std::execution::par_unseq,
                  view_divisors.begin(), view_divisors.end(),
                  [&inputs](const auto& divisor) {
        for (const auto& input : inputs) {
            if ((input % divisor) == 0) divisors[divisor]++;
        }
    });
}
// END: az sem

// Vnitrek funkce main nemente! Vyjimkou je jeden oznaceny blok nize
int main(int argc, char** argv)
{
    std::vector<int> randomInputs;

    // nagenerovane vstupy
    randomInputs.reserve(inputs_size);
    std::random_device rdev;
    std::default_random_engine reng(rdev());
    std::uniform_int_distribution<int> rdist(minnum, maxnum);

    for (size_t i = 0; i < inputs_size; i++)
        randomInputs.push_back(rdist(reng));

    // jeden "dry run" kvuli cache a tak podobne
    Do_Experiment(randomInputs);

    auto tp_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < experiment_run_count; i++)
        Do_Experiment(randomInputs);

    auto tp_end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tp_end - tp_start);

    auto totaltime = diff.count() / experiment_run_count;

    // BEGIN: Tady dle potreby muzete neco zmenit

    // drop first two divisors, 1 and 0
    auto view_divisors = divisors | std::views::drop(2);
    unsigned short i = 2;
    std::for_each(view_divisors.begin(), view_divisors.end(), [&i](const auto& it) {
        if (it != 0) {
            std::cout << i++ << " - " << it << " delitelu" << std::endl;
        }
    });
    // END: az sem

    std::cout << "Prumerny cas: " << totaltime << "ms" << std::endl;

    return 0;
}
