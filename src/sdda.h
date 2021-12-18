#pragma once

#include <array>
#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "core/CIFB.h"
#include "core/NIF.h"

// T: Arithmetic type representing the modulated signal
// Longer type gives better precision
// F: Interpolation filter type
// A: Maximum order of the modulator
template<typename T, typename M, typename F>
class SDDC{
  public:
    SDDC(M& modulator, F& filter);

    // Convert and write to an integer array
    inline void operator () (T input, uint8_t* out);

    // Convert samples from begin to end with delay compensation
    void operator () (T* begin, T* end, uint8_t* out);

    // Continuous conversion on a stream, preserving the state of the filter
    inline  uint8_t* convertStream(T* begin, T* end, uint8_t* out);
            uint8_t* getTailStream(uint8_t* out);

    // Continuous conversion on a stream, with downsampling
    inline  uint8_t* convertStream(T* begin, T* end, uint8_t* out, std::size_t DSR);

  private:
    M& _modulator;
    F& _filter;
    // Reserve buffer to avoid allocation each time
    std::vector<double> _overSampBuf;

    const std::size_t _OSR;
};

template<typename T, typename M, typename F>
SDDC<T, M, F>::SDDC(M& modulator, F& filter):
    _modulator(modulator),
    _filter(filter),
    _OSR(filter.getOSR()){
    _overSampBuf.resize(_OSR);
}


template<typename T, typename M, typename F>
void SDDC<T, M, F>::operator () (T input, uint8_t* out){
    _filter(input, _overSampBuf.data());
    for(std::size_t i = 0; i != _OSR; ++i){
        out[i] = _modulator(_overSampBuf[i]);
    }
}

template<typename T, typename M, typename F>
void SDDC<T, M, F>::operator () (T* begin, T* end, uint8_t* out){
    std::size_t delay = _filter.getDelay();
    T* start = std::min(end, begin + delay);

    for(; begin != start; ++begin){
        _filter(*(begin), _overSampBuf.data());
    }

    out = convertStream(begin, end, out);
    getTailStream(out);
}

template<typename T, typename M, typename F>
uint8_t* SDDC<T, M, F>::convertStream(T* begin, T* end, uint8_t* out){
    for(; begin != end; ++begin){
        (*this)(*begin, out);
        out+=_OSR;
    }

    return out;
}

template<typename T, typename M, typename F>
uint8_t* SDDC<T, M, F>::convertStream(T* begin, T* end, uint8_t* out, std::size_t DSR){
    for(; begin < end; ++begin){
        for(std::size_t i = 1; i != DSR; ++i){
            _filter(*(begin++), _overSampBuf.data());
        }

        (*this)(*begin, out);
        out+=_OSR;
    }

    return out;
}

template<typename T, typename M, typename F>
uint8_t* SDDC<T, M, F>::getTailStream(uint8_t* out){
    // Zero-padding the sequence to get the complete output
    for(std::size_t i = 0; i != _filter.getDelay(); ++i){
        (*this)(0, out);
        out+=_OSR;
    }

    return out;
}