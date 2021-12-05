
#include "../scaffold.h"
#include "lib/stemP.h"

constexpr std::size_t points = 32;
constexpr std::size_t OSR = 8;
constexpr double llimit = .0;
constexpr double ulimit = 3.14 * 4;

hold<double, OSR> holdF;
auto& filter = holdF;

std::array<double, points> samples;
std::array<double, points*OSR> oSamples;
std::array<uint8_t, points*OSR> modulated;
std::array<int, points*OSR> demodulated;

void test(double coef){
    sampler<double(double), points>(sin, llimit, ulimit, samples);
    DrawP(stemP(samples, llimit, ulimit));

    new TCanvas();

    double* oSampBeg = oSamples.data();
    for(size_t i = 0; i != points; ++i){
        filter(samples[i], &(oSamples[i * OSR]));
    }

    DrawP(stemP(oSamples, llimit, ulimit));


    new TCanvas();

    filter.reset();

    std::array<double, 2> coefs {coef, coef};

    SDDA<double, decltype(filter), 2> modulator(filter, coefs);
    for(size_t i = 0; i != points; ++i){
        modulator(samples[i], &(modulated[i * OSR]));
    }

    DrawP(stemP(modulated, llimit, ulimit));

    new TCanvas();

    int integral = 0;
    for(size_t i = 0; i != OSR*points; ++i){
        demodulated[i] = integral += (modulated[i] ? 1 : -1);
    }

    DrawP(stemP(demodulated, llimit, ulimit));
}
