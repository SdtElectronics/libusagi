/*
* Sigma-delta Modulator Core
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

#include <array>
#include <cstdint>
#include <exception>

// T: Arithmetic type representing the modulated signal
// A: Maximum order of the modulator
template <typename T, std::size_t A = 16>
class SDM{
  public:
    // coefs: Array of gains at each stage
    SDM(T* begin, T* end);

    // input: Oversampled data to be modulated
    uint8_t operator () (T input);

    // step: 
    void glitch(bool ov);

  private:
    T* _begin;
    T* _end;
    std::array<T, A> _integrator;
};

template <typename T, std::size_t A>
SDM<T, A>::SDM(T* begin, T* end): 
    _begin(begin),
    _end(end),
    _integrator(){
    if((end - begin) > A){
        throw std::length_error(
            "Designated modulation order is higher than maximum order supported");
    }
}

template <typename T, std::size_t A>
uint8_t SDM<T, A>::operator () (T input){
    auto integratorItr = _integrator.begin();
    bool ov = _integrator.back() > 0;
    for(T* coefPtr = _begin; coefPtr!= _end; ++coefPtr){
        T step = *coefPtr;
        step = ov ? step : -step;
        input -= step;
        input = *(integratorItr++) += input;
    }
    return static_cast<uint8_t>(ov);
}

template <typename T, std::size_t A>
void SDM<T, A>::glitch(bool ov){
    auto integratorItr = _integrator.begin();

    for(T* coefPtr = _begin; coefPtr!= _end; ++coefPtr){
        T step = *coefPtr;
        step = ov ? step : -step;
        *(integratorItr++) -= step;
    }
}

// TODO: Partial specialization for first order modulator