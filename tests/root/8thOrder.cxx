
#include "../scaffold.h"
#include "lib/stemP.h"

constexpr std::size_t points = 16;
constexpr std::size_t OSR = 16;
constexpr std::size_t llimit = 0;
constexpr double ulimit = 3.14 * 2;

finterp<double> fir = finterp<double>::createKaiserWin(OSR, 4, 60.f);
auto& filter = fir;

std::array<double, points> samples;
std::array<double, points*OSR> oSamples;
std::array<uint8_t, points*OSR> modulated;
//std::array<float, points*OSR> modulated;
std::array<int, points*OSR> demodulated;

void test(double coef){
    sampler<double(double), points>(sin, llimit, ulimit, samples);
    DrawP(stemP(samples, llimit));

    new TCanvas();

    double* oSampBeg = oSamples.data();
    
    for(size_t i = 0; i != points; ++i){
        filter(samples[i], &(oSamples[i * OSR]));
    }

    auto it = samples.begin();
    auto start = std::min(it + 4 + 1, samples.end());
    for(; it != start; ++it){
        filter(*(it), oSampBeg);
    }

    for(; it != samples.end(); ++it){
        (filter)(*it, oSampBeg+=OSR);
    }

    for(std::size_t i = 0; i != 4; ++i){
        (filter)(0, oSampBeg+=OSR);
    }

    DrawP(stemP(oSamples, llimit));


    new TCanvas();

    filter.reset();

    std::array<double, 8> coefs {
        -0.54184091028035863,
        4.6211255639903257,
        -17.319327891504788,
        37.237233820988472,
        -50.213208401223746,
        43.471176498498245,
        -23.588887732076959,
        7.3337285721047616};

    CIFB<double> modulator(&(*(coefs.begin())), &(*(coefs.end())));

    SDDC<double, decltype(modulator), decltype(filter)> converter(modulator, 
        filter);
    converter(samples.begin(), samples.end(), modulated.data());

    DrawP(stemP(modulated, llimit));

    new TCanvas();

    int integral = 0;
    for(size_t i = 0; i != OSR*points; ++i){
        demodulated[i] = integral += (modulated[i] ? 1 : -1);
    }

    DrawP(stemP(demodulated, llimit));
}
