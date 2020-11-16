/*
 * SDRDecimator.cpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#include "SDRDecimator.hpp"

namespace sdr {

SDRDecimator::SDRDecimator(const unsigned long long outFrequency,const unsigned long long inFrequency,
				 const unsigned blockSize,
				 const kfr::resample_quality quality) :
						 factor(outFrequency/(float)inFrequency),
						 outF(outFrequency), inF(inFrequency),
						 decimatorR(kfr::resampler<float>(quality,outF,inF)),
						 decimatorI(kfr::resampler<float>(quality,outF,inF)),
						 inBlock(blockSize*std::ceil(factor)),
						 				outBlock(blockSize),
						 				inputs(inBlock),
						 				outputs(outBlock)
										{}



SDRDecimator::~SDRDecimator() {


}

void SDRDecimator::operator()(const std::complex<float> *in) {

	std::cout << "    DEC Outblock = " << outBlock << " Inblock = " << inBlock  << std::endl;
	inputs.load(in);
	std::cout << "     DEC Decimating" << std::endl;
	decimatorR.process(outputs.real,inputs.real);
	decimatorI.process(outputs.imag,inputs.imag);
	std::cout << "     DEC Completed" << std::endl;
}

void SDRDecimator::operator()(const std::vector<cx_t> &in) {
	(*this)(in.data());
}


} /* namespace sdr */
