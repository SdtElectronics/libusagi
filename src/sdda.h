#pragma once

#include <array>
#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

#include <cstdio>

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
    inline void operator () (T input, uint8_t* out);

/*
    // Convert and write to an integer array
    inline void operator () (T input, float* out);
*/

    // Convert samples from begin to end with delay compensation
    void operator () (T* begin, T* end, uint8_t* out);

    //void operator () (T* begin, T* end, float* out);

    inline  uint8_t* convertContinuous(T* begin, T* end, uint8_t* out);
            uint8_t* getTailContinuous(uint8_t* out);

  private:
    F& _filter;
    SDM<T, A> _modulator;
    // Reserve buffer to avoid allocation each time
    std::vector<double> _overSampBuf;

    const std::size_t _OSR;
};

template<typename T, typename F, std::size_t A>
SDDA<T, F, A>::SDDA(F& filter, const std::array<T, A>& coefs):
    _filter(filter),
    _modulator(coefs),
    _OSR(filter.getOSR()){
    _overSampBuf.resize(_OSR);
}


template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T input, uint8_t* out){
    _filter(input, _overSampBuf.data());
    for(std::size_t i = 0; i != _OSR; ++i){
        out[i] = _modulator(_overSampBuf[i]);
    }
}

/*
template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T input, float* out){
    _filter(input, _overSampBuf.data());
    for(std::size_t i = 0; i != _OSR; ++i){
        out[i] = _overSampBuf[i];
    }
}
*/

template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T* begin, T* end, uint8_t* out){
    std::size_t delay = _filter.getDelay();
    T* start = std::min(end, begin + delay);

    for(; begin != start; ++begin){
        _filter(*(begin), _overSampBuf.data());
    }

    out = convertContinuous(begin, end, out);
    getTailContinuous(out);
}

template<typename T, typename F, std::size_t A>
uint8_t* SDDA<T, F, A>::convertContinuous(T* begin, T* end, uint8_t* out){
    for(; begin != end; ++begin){
        (*this)(*begin, out);
        out+=_OSR;
    }

    return out;
}

template<typename T, typename F, std::size_t A>
uint8_t* SDDA<T, F, A>::getTailContinuous(uint8_t* out){
    // Zero-padding the sequence to get the complete output
    for(std::size_t i = 0; i != _filter.getDelay(); ++i){
        (*this)(0, out);
        out+=_OSR;
    }

    return out;
}

/*
template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T* begin, T* end, uint8_t* out){
    std::size_t delay = _filter.getDelay();
    T* final = begin + delay;

    if(end > final){
        // delay is shorter than the signal sequence
        // Skip the delay in the output
        for(; begin != final; ++begin){
            _filter(*(begin), _overSampBuf.data());
        }

        // Push the rest of the sequence into the modulator
        for(; begin != end; ++begin){
            (*this)(*begin, out);
            out+=OSR;
        }

        // Zero-padding the sequence to get the complete output
        for(std::size_t i = 0; i != delay; ++i){
            (*this)(0, out);
            out+=OSR;
        }
    }else{
        // delay is longer than the signal sequence
        std::size_t padding = final - end;

        // Push the the complete sequence into the modulator
        for(; begin != end; ++begin){
            _filter(*(begin), _overSampBuf.data());
        }

        // Zero-padding the sequence to get the complete output,
        // skip the delay in the output
        for(std::size_t i = 0; i != padding; ++i){
            (*this)(0, out);
        }

        for(std::size_t i = 0; i != (OSR - padding); ++i){
            (*this)(0, out);
            out+=OSR;
        }
    }
}

template<typename T, typename F, std::size_t A>
void SDDA<T, F, A>::operator () (T* begin, T* end, float* out){
    std::size_t delay = _filter.getDelay();
    T* final = begin + delay;

    if(end > final){
        // delay is shorter than the signal sequence
        // Skip the delay in the output
        for(; begin != final; ++begin){
            _filter(*(begin), _overSampBuf.data());
        }

        // Push the rest of the sequence into the modulator
        for(; begin != end; ++begin){
            (*this)(*begin, out);
            out+=OSR;
        }

        // Zero-padding the sequence to get the complete output
        for(std::size_t i = 0; i != delay; ++i){
            (*this)(0, out);
            out+=OSR;
        }
    }else{
        // delay is longer than the signal sequence
        std::size_t padding = final - end;

        // Push the the complete sequence into the modulator
        for(; begin != end; ++begin){
            _filter(*(begin), _overSampBuf.data());
        }

        // Zero-padding the sequence to get the complete output,
        // skip the delay in the output
        for(std::size_t i = 0; i != padding; ++i){
            (*this)(0, out);
        }

        for(std::size_t i = 0; i != (OSR - padding); ++i){
            (*this)(0, out);
            out+=OSR;
        }
    }
}
*/