/*
* FIR filter in liquid
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

#include <vector>

#include <liquid/liquid.h>

template<typename T>
class firfilt{
  public:
    firfilt(firfilt_rrrf firfilt);
    firfilt(T* Coefs, std::size_t CoefNum);

    // liquid DSP does not support copy of firfilt_rrrf
    // thus the copy constructor of this wrapper class is deleted
    firfilt(const firfilt&) = delete;

    // liquid DSP does not support copy of firfilt_rrrf
    // thus the assignment operator of this wrapper class is deleted
    firfilt& operator = (const firfilt&) = delete;
    
    // Interface
    inline T operator () (T val);
    inline std::size_t getDelay() const;
    void reset();

    ~firfilt();

    // Factory methods
    static firfilt createKaiserWin(std::size_t order, float fc, float As);

  private:
    firfilt_rrrf _firfilt;
    const std::size_t _delay;
};

template<typename T>
firfilt<T>::firfilt(firfilt_rrrf firfilt):
    _firfilt(firfilt),
    _delay(firfilt_rrrf_get_length(_firfilt)/2){
}

template<typename T>
firfilt<T>::firfilt(T* Coefs, std::size_t CoefNum):
    firfilt(firfilt_rrrf_create(Coefs, CoefNum)){
}


template<typename T>
firfilt<T> firfilt<T>::createKaiserWin(std::size_t order, float fc, float As){
    std::vector<float> coefs(order);
    liquid_firdes_kaiser(order, fc, As, 0.0f, coefs.data());

    return firfilt<T>(firfilt_rrrf_create(coefs.data(), order));
}

template<typename T>
T firfilt<T>::operator () (T val){
    float y;
    firfilt_rrrf_push(_firfilt, val);
    firfilt_rrrf_execute(_firfilt, &y);
    return y;
}

template<typename T>
std::size_t firfilt<T>::getDelay() const{
    return _delay;
}

template<typename T>
void firfilt<T>::reset(){
    firfilt_rrrf_reset(_firfilt);
}

template<typename T>
firfilt<T>::~firfilt(){
    firfilt_rrrf_destroy(_firfilt);
}