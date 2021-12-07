/*
* Oversampling with FIR filter in liquid
*
* Copyright (c) 2021 SdtElectronics . All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from this
*    software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <complex>
#include <vector>

#include <liquid/liquid.h>

template<typename T>
class finterp{
  public:
    finterp(firinterp_crcf firinterp);
    finterp(std::size_t OSR, T* coefs, std::size_t length);
    
    // Interface
    inline void operator () (T val, T* out);
    inline std::size_t getOSR();
    inline std::size_t getDelay();
    void reset();

    ~finterp();

    // Factory methods
    static finterp createKaiserWin(std::size_t OSR, std::size_t m, float As);

  private:
    const std::size_t _OSR;
    const std::size_t _delay;
    firinterp_crcf _firinterp;

    // Reserve buffer to avoid allocation each time
    std::vector<std::complex<float>> _buf;
};

template<typename T>
finterp<T>::finterp(firinterp_crcf firinterp):
    _firinterp(firinterp),
    _OSR(firinterp_crcf_get_interp_rate(firinterp)),
    _delay(firinterp_crcf_get_sub_len(firinterp)/2){
    _buf.resize(_OSR);
}

template<typename T>
finterp<T>::finterp(std::size_t OSR, T* coefs, std::size_t length):
    finterp(firinterp_crcf_create(OSR, coefs, length)){
}

template<typename T>
finterp<T> finterp<T>::createKaiserWin(std::size_t OSR, std::size_t m, float As){
    return finterp(firinterp_crcf_create_kaiser(OSR, m, As));
}

template<typename T>
void finterp<T>::operator () (T val, T* out){
    std::complex<float> x {static_cast<float>(val), 0};
    firinterp_crcf_execute(_firinterp, x, _buf.data());
    for(std::size_t i = 0; i != _OSR; ++i){
        out[i] = static_cast<T>(_buf[i].real());
    }
}

template<typename T>
std::size_t finterp<T>::getOSR(){
    return _OSR;
}

template<typename T>
std::size_t finterp<T>::getDelay(){
    return _delay;
}

template<typename T>
void finterp<T>::reset(){
    firinterp_crcf_reset(_firinterp);
}


template<typename T>
finterp<T>::~finterp(){
    firinterp_crcf_destroy(_firinterp);
}