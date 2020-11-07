/*
 * AUStream.cpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#include "AUStream.hpp"

namespace audio {

unsigned long AUStreamParameters::sampleSize() const {
	auto sampleSize = Pa_GetSampleSize(sampleFormat());
	if(sampleSize==paSampleFormatNotSupported) throw AUError("Sample format not supported");
	return (unsigned long)sampleSize;
}




AUStream::AUStream(const AUDevice &device,const AUStreamParameters &params) : active(false) {

	PaStreamParameters p;
	bzero(&p,sizeof(p));
	p.channelCount=params.nChannels;
	p.device=device.index();
	p.hostApiSpecificStreamInfo=NULL;
	p.sampleFormat=params.sampleFormat();
	p.suggestedLatency = device.latencyOut();
	p.hostApiSpecificStreamInfo=NULL;
	fpb = params.bufferSize/params.nChannels;

	PaStream *stream_;
	AUError::check(Pa_OpenStream(&stream_,nullptr,&p,params.rate,fpb,paNoFlag,nullptr,nullptr));
	stream=stream_;
}
AUStream::AUStream(const AUStreamParameters &params) : AUStream(AUDevice(),params) {}

AUStream::~AUStream() {
	if(stream!=nullptr) {
		try {
			Pa_CloseStream(stream);
			stream=nullptr;
		}
		catch(...) {}
	}
}

void AUStream::start() {
	if(!active) {
		AUError::check(Pa_StartStream(stream));
		active=true;
	}
}

void AUStream::stop() {
	if(active) {
		active=false;
		AUError::check(Pa_StopStream(stream));
	}
}

void AUStream::forceStop() {
	if(active) {
		active=false;
		AUError::check(Pa_AbortStream(stream));
	}
}

void AUStream::write(float *buffer) {
	if(active) {
		AUError::check(Pa_WriteStream(stream,buffer,fpb));
	}
}

} /* namespace audio */
