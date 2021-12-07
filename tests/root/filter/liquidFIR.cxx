#include <vector>

#include "../../scaffold.h"
#include "../lib/stemP.h"

constexpr std::size_t points = 16;
constexpr std::size_t OSR = 16;
constexpr std::size_t llimit = 0;
constexpr double ulimit = 3.14 * 2;

finterp<double> fir = finterp<double>::createKaiserWin(OSR, 4, 60.f);
auto& filter = fir;

const std::size_t delay = fir.getDelay();

std::array<double, points> samples;
std::vector<double> oSamples;
std::array<uint8_t, points*OSR> modulated;
//std::array<float, points*OSR> modulated;
std::array<int, points*OSR> demodulated;

void test(double coef){
    sampler<double(double), points>(sin, llimit, ulimit, samples);
    DrawP(stemP(samples, llimit));

    new TCanvas();

    oSamples.resize((points + delay)*OSR);
    double* oSampBeg = oSamples.data();
    
    for(auto it = samples.begin(); it != samples.end(); ++it){
        filter(*it, oSampBeg);
        oSampBeg+=OSR;
    }

    for(std::size_t i = 0; i != delay; ++i){
        filter(0, oSampBeg);
        oSampBeg+=OSR;
    }

    DrawP(stemP(oSamples.begin() + delay*OSR, oSamples.end(), llimit));


    new TCanvas();

    filter.reset();

    std::array<double, 1> coefs {coef};

    SDDA<double, decltype(filter), 1> modulator(filter, coefs);
    modulator(samples.begin(), samples.end(), modulated.data());

    DrawP(stemP(modulated, llimit));

    new TCanvas();

    int integral = 0;
    for(size_t i = 0; i != OSR*points; ++i){
        demodulated[i] = integral += (modulated[i] ? 1 : -1);
    }

    DrawP(stemP(demodulated, llimit));
}
