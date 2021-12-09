
#include "../../scaffold.h"
#include "../lib/stemP.h"

constexpr std::size_t points = 16;
constexpr std::size_t OSR = 16;
constexpr std::size_t llimit = 0;
constexpr double ulimit = 3.14 * 2;

hold<double> holdF(OSR);
auto& filter = holdF;

std::array<double, points> samples;
std::array<double, points*OSR> oSamples;
std::array<uint8_t, points*OSR> modulated;
std::array<int, points*OSR> demodulated;

void test(double coef){
    sampler<double(double), points>(sin, llimit, ulimit, samples);
    DrawP(stemP(samples, llimit));

    new TCanvas();

    double* oSampBeg = oSamples.data();
    for(size_t i = 0; i != points; ++i){
        filter(samples[i], &(oSamples[i * OSR]));
    }

    DrawP(stemP(oSamples, llimit));


    new TCanvas();

    filter.reset();

    std::array<double, 1> coefs {coef};

    SDDC<double, decltype(filter), 1> modulator(filter, 
        &(*(coefs.begin())),
        &(*(coefs.end())));
    modulator(samples.begin(), samples.end(), modulated.data());

    DrawP(stemP(modulated, llimit));

    new TCanvas();

    int integral = 0;
    for(size_t i = 0; i != OSR*points; ++i){
        demodulated[i] = integral += (modulated[i] ? 1 : -1);
    }

    DrawP(stemP(demodulated, llimit));
}
