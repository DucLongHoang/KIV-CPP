#include "generator.hpp"

#include "shared.hpp"

#include <fstream>
#include <random>

void Generator::Generate()
{
    std::ofstream ofs(InFileName, std::ios::out | std::ios::binary);

    std::random_device rdev;
    std::default_random_engine reng(rdev());
    std::normal_distribution<double> rdist(mMean, mSigma);

    for (size_t i = 0; i < mNumber_Count; i++)
    {
        double d = std::max(mMin, std::min(mMax, rdist(reng)));
        ofs.write((char*)&d, sizeof(double));
    }
}
