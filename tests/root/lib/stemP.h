/*
* Plot stem graph
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

#include <cstdint>
#include <array>
#include <memory>

template <typename T, std::size_t L>
auto stemP(const std::array<T, L>& arr, double ll, double ul, const char* name = "", const char* opt = "aps"){
	//auto len = static_cast<std::size_t>(++ul - ll);
	double ax[L], ayh[L], ayl[L], ay[L], axz[L];
    /*
	for(double ix = ll; ix != ul; ++ix){
		int ind = ix -ll;
		axz[ind] = 0;
		ax[ind] = ix;
		double iy = arr[ind];
		ix = ax[ind];
		if(iy < 0){
			ayh[ind] = -iy;
			ayl[ind] = 0;
		}else{
			ayh[ind] = 0;
			ayl[ind] = iy;
		}
		ay[ind] = iy;
	}
    */
    for(std::size_t ix = 0; ix != L; ++ix){
		int ind = ix;
		axz[ind] = 0;
		ax[ind] = ix;
		double iy = arr[ind];
		if(iy < 0){
			ayh[ind] = -iy;
			ayl[ind] = 0;
		}else{
			ayh[ind] = 0;
			ayl[ind] = iy;
		}
		ay[ind] = iy;
	}
	auto ret = std::shared_ptr<TGraphAsymmErrors>(new TGraphAsymmErrors(L, ax, ay, axz, axz, ayl, ayh));
	ret->SetLineColor(4);
	ret->SetMarkerColor(4);
	ret->SetTitle(name);
	ret->SetMarkerStyle(4);
	return ret;
}

auto DrawP(const std::shared_ptr<TGraphAsymmErrors> objPtr, const char* opt = "aps"){
	objPtr->Draw(opt);
	return new shared_ptr<TGraphAsymmErrors>(objPtr);
}