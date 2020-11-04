/*
 * SDRDecimator.cpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#include "SDRDecimator.hpp"

namespace sdr {

SDRDecimator::SDRDecimator(const unsigned factor_,const unsigned blockSize_,
		const float attenuation_,const unsigned delay_) :
				factor(factor_), blockSize(blockSize_),
				attenuation(attenuation_), delay(delay_) {
	outputs = new std::complex<float>[blockSize];
	decimator = firdecim_crcf_create_kaiser(factor,delay,attenuation);
	firdecim_crcf_set_scale(decimator, 1.0f/(float)factor);
}

SDRDecimator::~SDRDecimator() {
	delete[] outputs;
	if(decimator) firdecim_crcf_destroy(decimator);
}

std::complex<float> * SDRDecimator::operator()(std::complex<float> *inputs) {
	firdecim_crcf_execute_block(decimator,inputs,blockSize,outputs);
	return outputs;
}


} /* namespace sdr */
