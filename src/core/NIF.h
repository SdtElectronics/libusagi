/*
* Modulator without Input Feedforward
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
// F: Noise-shaping filter type
template <typename T, typename F>
class NIF{
  public:
    NIF(F& filter, T deltaGain);

    // input: Oversampled data to be modulated
    uint8_t operator () (T input);

    std::size_t getDelay();

  private:
    F& _filter;
    T _deltaGain;
    bool ov = false;
};

template <typename T, typename F>
NIF<T, F>::NIF(F& filter, T deltaGain): _filter(filter), _deltaGain(deltaGain){
}

template <typename T, typename F>
uint8_t NIF<T, F>::operator () (T input){
    T step = _deltaGain;
    step = ov ? step : -step;
    ov = _filter(input - step) > 0;
    return static_cast<uint8_t>(ov);
}

template <typename T, typename F>
std::size_t NIF<T, F>::getDelay(){
    return _filter.getDelay();
}