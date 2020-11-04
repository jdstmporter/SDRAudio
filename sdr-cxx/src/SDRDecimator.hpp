/*
 * SDRDecimator.hpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRDECIMATOR_HPP_
#define SDRDECIMATOR_HPP_

#include <complex>
#include <liquid/liquid.h>

namespace sdr {

class SDRDecimator {
private:
	unsigned factor;
	unsigned blockSize;
	float attenuation;
	unsigned delay;
	std::complex<float> *outputs;
	firdecim_crcf decimator;
public:
	using iterator = std::complex<float> *;
	using const_iterator = const std::complex<float> *;
	SDRDecimator(const unsigned factor_,const unsigned blockSize_,
					const float attenuation_=60.0,const unsigned delay_=8);
	SDRDecimator() : SDRDecimator(32,32) {};
	virtual ~SDRDecimator();
	SDRDecimator(SDRDecimator &&other) = default;
	SDRDecimator(const SDRDecimator &other) = default;


	std::complex<float> * operator()(std::complex<float> *inputs);

	iterator begin() { return outputs; }
	iterator end() { return outputs+blockSize; }
	const_iterator begin() const { return outputs; }
	const_iterator end() const { return outputs+blockSize; }
	unsigned size() const { return blockSize; }
};

} /* namespace sdr */

#endif /* SDRDECIMATOR_HPP_ */
