/*
 * SDRDecimator.cpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#include "SDRDecimator.hpp"

namespace sdr {

void SDRSimpleDecimator::operator()(const std::complex<float> *in) {
	for(unsigned i=0;i<outBlock;i++) outputs.set(i,in[i*factor]);
}

void SDRSimpleLPFDecimator::operator()(const std::complex<float> *in) {

	for(unsigned i=0;i<outBlock;i++) {
		float mean=0;
		auto offset=i*factor;
		for(unsigned j=0;i<factor;j++) mean+=in[offset+j];
		outputs.set(i,mean/(float)factor);
	}
}

SDRDecimator::SDRDecimator(
				 const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_,
				 const kfr::resample_quality quality) :
		SDRDecimatorBase(inFrequency,blockSize,factor_),
		decimatorR(kfr::resampler<float>(quality,(size_t)outF,(size_t)inF)),
		decimatorI(kfr::resampler<float>(quality,(size_t)outF,(size_t)inF)) {};

SDRDecimator::SDRDecimator(const float inFrequency,
				 const float outFrequency,
				 const unsigned long blockSize,
				 const kfr::resample_quality quality) :
		SDRDecimatorBase(inFrequency,outFrequency,blockSize),
		decimatorR(kfr::resampler<float>(quality,(size_t)outF,(size_t)inF)),
		decimatorI(kfr::resampler<float>(quality,(size_t)outF,(size_t)inF)) {};




void SDRDecimator::operator()(const std::complex<float> *in) {
	inputs.load(in);
	decimatorR.process(outputs.real,inputs.real);
	decimatorI.process(outputs.imag,inputs.imag);
}


} /* namespace sdr */
