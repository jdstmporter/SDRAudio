/*
 * base.hpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#ifndef BASE_HPP_
#define BASE_HPP_

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <complex>
#include <vector>
#include <algorithm>
#include <iterator>

#include <kfr/base.hpp>
#include <kfr/dsp.hpp>

namespace sdr {
using cx_t = std::complex<float>;
using vec_t = kfr::univector<float>;
using freq_t = unsigned long long;

constexpr cx_t cx_zero = cx_t(0,0);
constexpr cx_t cx_one = cx_t(1,0);
constexpr cx_t cx_i = cx_t(0,1);

constexpr float PI = 3.14159256;

struct SDRConstants {
	static constexpr freq_t AUDIO_RATE = 48000;
	static constexpr freq_t RF_RATE = 1536000;
	static constexpr freq_t DECIMATION_FACTOR = RF_RATE/AUDIO_RATE;
	static constexpr freq_t AUDIO_BLOCK_SIZE = 1024;
	static constexpr freq_t RF_BLOCK_SIZE = AUDIO_BLOCK_SIZE * DECIMATION_FACTOR;

	static void print() {
		std::cout << "Audio rate = " << AUDIO_RATE << " block = " << AUDIO_BLOCK_SIZE << std::endl;
		std::cout << "RF    rate = " << RF_RATE    << " block = " << RF_BLOCK_SIZE << std::endl;
		std::cout << "FACTOR     = " << DECIMATION_FACTOR << std::endl;
	}
};

cx_t r2cx(const float) noexcept;

struct CXData {



	freq_t n;
	vec_t real;
	vec_t imag;

	CXData(const std::vector<cx_t> &inputs);
	CXData(const cx_t *inputs,const freq_t n_);
	CXData(const freq_t n_) : n(n_), real(n,0), imag(n,0) {};
	CXData(const CXData &) = default;
	CXData & operator=(const CXData &) = default;
	virtual ~CXData() = default;

	void load(const cx_t *inputs);
	void load(const std::vector<cx_t> &inputs);

	freq_t size() const { return n; }
	vec_t::iterator beginR() { return real.begin(); }
	vec_t::iterator beginI() { return imag.begin(); }
	vec_t::iterator endR() { return real.end(); }
	vec_t::iterator endI() { return imag.end(); }

	//iterator begin() { return iterator(*this); }
	//iterator end() { return iterator(*this,n); }

	float operator[](const freq_t idx) const;

};




}




#endif /* BASE_HPP_ */
