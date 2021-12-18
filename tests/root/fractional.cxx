#include <vector>

#include "../scaffold.h"
#include "lib/stemP.h"

constexpr std::size_t points = 16;
constexpr std::size_t OSR = 16;
constexpr std::size_t llimit = 0;
constexpr double ulimit = 3.14 * 2;

finterp<double> fir = finterp<double>::createKaiserWin(OSR, 4, 60.f);
auto& filter = fir;

const std::size_t delay = fir.getDelay();

std::array<double, points> samples;
std::vector<double> oSamples;
std::vector<uint8_t> modulated;
//std::array<float, points*OSR> modulated;
std::vector<int> demodulated;

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

    SDDC<double, decltype(filter), 1> modulator(filter, 
        &(*(coefs.begin())),
        &(*(coefs.end())));
    modulated.resize((points + delay)*OSR);

    auto tail = modulator.convertStream(samples.begin(), samples.end() - 6, modulated.data());
    tail = modulator.convertStream(samples.end() - 6, samples.end(), tail);
    modulator.getTailStream(tail);

    DrawP(stemP(modulated.begin() + delay*OSR, modulated.end(), llimit));


    new TCanvas();

    demodulated.resize((points + delay)*OSR);

    int integral = 0;
    for(size_t i = (delay * OSR); i != (delay + points)*OSR; ++i){
        demodulated[i] = integral += (modulated[i] ? 1 : -1);
    }

    DrawP(stemP(demodulated.begin() + delay*OSR, demodulated.end(), llimit));
}
