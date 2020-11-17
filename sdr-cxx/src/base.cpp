/*
 * base.cpp
 *
 *  Created on: 14 Nov 2020
 *      Author: julianporter
 */


#include "base.hpp"

namespace sdr {
//constexpr cx_t cx_zero = cx_t(0,0);
//constexpr cx_t cx_one = cx_t(1,0);
//constexpr cx_t cx_i = cx_t(0,1);


cx_t r2cx(const float f) noexcept { return cx_t(f,0); }




CXData::CXData(const std::vector<cx_t> &inputs) :
					CXData(inputs.data(),inputs.size()) {}

CXData::CXData(const cx_t *inputs,const freq_t n_) :
				CXData(n_) {
	auto itr=real.begin();
	auto iti=imag.begin();
	for(auto i=0;i<n;i++) {
		auto cx = inputs[i];
		*itr++ = std::real(cx);
		*iti++ = std::imag(cx);
	}


}

void CXData::load(const cx_t *inputs) {
	auto itr=real.begin();
	auto iti=imag.begin();
	for(auto i=0;i<n;i++) {
		auto cx = inputs[i];
		*itr++ = std::real(cx);
		*iti++ = std::imag(cx);
	}
}

void CXData::load(const std::vector<cx_t> &inputs) {
	load(inputs.data());
}

float CXData::operator[](const freq_t idx) const {
	auto pos=idx/2;
	if((idx & 1)==0) return real[pos];
	else return imag[pos];
}

/*
float CXData::iterator::at() const {
	if((offset & 1)==0) return *reals;
	else return *imags;
}

CXData::iterator & CXData::iterator::operator=(const iterator &o) {
		this->offset = o.offset;
		this->size = o.size;
		this->reals = o.reals;
		this->imags = o.imags;
		return *this;
	}
CXData::iterator & CXData::iterator::operator++() {
	if((offset &&1) == 0) ++reals;
	else ++imags;
	++offset;
	return *this;
}
CXData::iterator & CXData::iterator::operator++(int) {
	auto o=*this;
	if((offset &&1) == 0) ++reals;
	else ++imags;
	++offset;
	return o;
}
*/

}
