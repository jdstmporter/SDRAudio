/*
 * AUStreamParameters.cpp
 *
 *  Created on: 10 Nov 2020
 *      Author: julianporter
 */
#include "AUStreamParameters.hpp"

namespace audio {

unsigned long AUStreamParameters::sampleSize() const {
	auto sampleSize = Pa_GetSampleSize(sampleFormat());
	if(sampleSize==paSampleFormatNotSupported) throw AUError("Sample format not supported");
	return (unsigned long)sampleSize;
}

AUStreamParameters::params_t AUStreamParameters::streamParameters(const AUDevice &d) const {
		PaStreamParameters p;
		bzero(&p,sizeof(p));
		p.channelCount=nChannels;
		p.device=d.index();
		p.hostApiSpecificStreamInfo=NULL;
		p.sampleFormat=sampleFormat();
		p.suggestedLatency = d.latencyOut();
		p.hostApiSpecificStreamInfo=NULL;
		return p;
	}

}



