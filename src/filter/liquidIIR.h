/*
* IIR filter in liquid
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
class iirfilt{
  public:
    iirfilt(iirfilt_crcf iirfilt);
    iirfilt(T* ffCoefs, std::size_t ffCoefNum, T* fbCoefs, std::size_t fbCoefNum);

    // liquid DSP does not support copy of iirfilt_crcf
    // thus the copy constructor of this wrapper class is deleted
    iirfilt(const iirfilt&) = delete;

    // liquid DSP does not support copy of iirfilt_crcf
    // thus the assignment operator of this wrapper class is deleted
    iirfilt& operator = (const iirfilt&) = delete;
    
    // Interface
    inline T operator () (T val);
    inline std::size_t getDelay() const;
    void reset();

    ~iirfilt();

    // Factory methods
    static iirfilt createChebyI(std::size_t order, float fc, float Ap);
    static iirfilt createChebyII(std::size_t order, float fc, float As);

  private:
    iirfilt_crcf _iirfilt;
    const std::size_t _delay;
};

template<typename T>
iirfilt<T>::iirfilt(iirfilt_crcf iirfilt):
    _iirfilt(iirfilt),
    _delay(iirfilt_crcf_get_length(_iirfilt)/2){
}

template<typename T>
iirfilt<T>::iirfilt(T* ffCoefs, std::size_t ffCoefNum, T* fbCoefs, std::size_t fbCoefNum):
    iirfilt(iirfilt_crcf_create(ffCoefs, ffCoefNum, fbCoefs, fbCoefNum)){
}


template<typename T>
iirfilt<T> iirfilt<T>::createChebyI(std::size_t order, float fc, float Ap){
    return iirfilt<T>(iirfilt_crcf_create_prototype(LIQUID_IIRDES_CHEBY1, 
                                                LIQUID_IIRDES_LOWPASS, 
                                                LIQUID_IIRDES_SOS, 
                                                order, fc, 0, Ap, 1));
}

template<typename T>
iirfilt<T> iirfilt<T>::createChebyII(std::size_t order, float fc, float As){
    return iirfilt<T>(iirfilt_crcf_create_prototype(LIQUID_IIRDES_CHEBY2, 
                                                LIQUID_IIRDES_LOWPASS, 
                                                LIQUID_IIRDES_SOS, 
                                                order, fc, 0, 1, As));
}

template<typename T>
T iirfilt<T>::operator () (T val){
    std::complex<float> x {static_cast<float>(val), 0};
    std::complex<float> y;
    iirfilt_crcf_execute(_iirfilt, x, &y);
    return y.real();
}


template<typename T>
std::size_t iirfilt<T>::getDelay() const{
    return _delay;
}

template<typename T>
void iirfilt<T>::reset(){
    iirfilt_crcf_reset(_iirfilt);
}


template<typename T>
iirfilt<T>::~iirfilt(){
    iirfilt_crcf_destroy(_iirfilt);
}