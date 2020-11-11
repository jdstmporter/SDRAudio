/*
 * AUStreamParameters.hpp
 *
 *  Created on: 10 Nov 2020
 *      Author: julianporter
 */

#ifndef AUSTREAMPARAMETERS_HPP_
#define AUSTREAMPARAMETERS_HPP_

#include <portaudio.h>
#include "AUBase.hpp"
#include "AUDevice.hpp"

namespace audio {

struct AUStreamParameters {
	using sample_t = PaSampleFormat;
	using params_t = PaStreamParameters;

	unsigned nChannels;
	AUSampleType format;
	double rate;
	unsigned bufferSize;

	AUStreamParameters(const unsigned nChannels_=2,
					const AUSampleType format_=AUSampleType::Float,
					const double rate_=4.8e4,
					const unsigned bufferSize_=2048) :
		nChannels(nChannels_),format(format_),rate(rate_),bufferSize(bufferSize_) {};
	virtual ~AUStreamParameters()=default;

	unsigned long sampleSize() const;
	unsigned long frameSize() const { return sampleSize()*nChannels; }
	sample_t sampleFormat() const { return static_cast<sample_t>(format); }

	params_t streamParameters(const AUDevice &) const;
};
}



#endif /* AUSTREAMPARAMETERS_HPP_ */
