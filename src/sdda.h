#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

#include "core/sdm.h"

// T: Arithmetic type representing the modulated signal
// Longer type gives better precision
// F: Interpolation filter object
// A: Order of the modulator
template<typename T, typename F, std::size_t A>
class SDDA{
  public:
    SDDA(F& filter, const std::array<T, A>& coefs);

    // Convert and write to an integer array
    void operator () (T input, uint8_t* out);

  private:
    static constexpr std::size_t OSR = std::decay<F>::type::length;

    F& _filter;
    SDM<T, A> _modulator;
    std::array<double, OSR> _overSampBuf;
};

template<typename T, typename F, std::size_t A>
SDDA<T, F, A>::SDDA(F& filter, const std::array<T, A>& coefs):
    _filter(filter),
    _modulator(coefs){
    
}


template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T input, uint8_t* out){
    _filter(input, _overSampBuf.data());
    for(std::size_t i = 0; i != OSR; ++i){
        out[i] = _modulator(_overSampBuf[i]);
    }
}