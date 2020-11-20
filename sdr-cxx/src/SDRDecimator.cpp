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

void SDRSimpleLPFDecimator::makeFilter() {
	float ff = (float)factor;
	auto f = sdr::PI/ff;
	coeffts[factor-1]=1.0f/ff;
	for(auto i=1;i<factor;i++) {
		coeffts[factor-1-i]=sin(f*i)/(sdr::PI*i);
	}
	for(auto i=0;i<factor;i++) std :: cout << coeffts[i] << " ";
	std::cout << std::endl;
}

SDRSimpleLPFDecimator::SDRSimpleLPFDecimator(const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_) :
		SDRDecimatorBase(inFrequency,blockSize,factor_), coeffts(factor) {
	makeFilter();
};
SDRSimpleLPFDecimator::SDRSimpleLPFDecimator(const float inFrequency,
				 const float outFrequency,
				 const unsigned long blockSize) :
		SDRDecimatorBase(inFrequency,outFrequency,blockSize), coeffts(factor) {
	makeFilter();
};

void SDRSimpleLPFDecimator::operator()(const std::complex<float> *in) {

	for(unsigned i=0;i<outBlock;i++) {
		cx_t filtered=0;
		auto offset=i*factor;
		for(unsigned j=0;j<factor;j++) {
			//std::cout << i << " " << j << " " << coeffts[j] << std::endl;
			filtered+=in[offset+j]*coeffts[j];
		}
		outputs.set(i,filtered);
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
