/*
 * AUStream.cpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#include "AUStream.hpp"
#include <algorithm>

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




AUStreamBase::AUStreamBase(const AUDevice &device_,const AUStreamParameters &params_)
: active(false), bufferSize(params.bufferSize), stream(nullptr), params(params_), device(device_) {
	fpb = params.bufferSize/params.nChannels;
}

AUStreamBase::~AUStreamBase() {
	if(stream!=nullptr) {
		try {
			Pa_CloseStream(stream);
			stream=nullptr;
		}
		catch(...) {}
	}
}

void AUStreamBase::open() {
	PaStream *stream_;
	PaStreamParameters p=params.streamParameters(device);
	AUError::check(Pa_OpenStream(&stream_,nullptr,&p,params.rate,fpb,paNoFlag,nullptr,nullptr));
	stream=stream_;
}

void AUStreamBase::start() {
	if(!active) {
		AUError::check(Pa_StartStream(stream));
		active=true;
	}
}

void AUStreamBase::stop() {
	if(active) {
		active=false;
		AUError::check(Pa_StopStream(stream));
	}
}

void AUStreamBase::forceStop() {
	if(active) {
		active=false;
		AUError::check(Pa_AbortStream(stream));
	}
}

void AUStream::write(const std::vector<float> &vec) {
	if(active) {
		AUError::check(Pa_WriteStream(stream,vec.data(),vec.size()));
	}
}

static int nonBlockingCallback(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,void *userData) {
	auto me = (AUNonBlockingStream *)userData;
	me->getNextBlock(outputBuffer,framesPerBuffer);
	return 0;
}

void AUNonBlockingStream::open() {
	PaStream *stream_;
	PaStreamParameters p=params.streamParameters(device);
	AUError::check(Pa_OpenStream(&stream_,nullptr,&p,params.rate,fpb,paNoFlag,nonBlockingCallback,(void *)this));
	stream=stream_;
}

void AUNonBlockingStream::getNextBlock(void *outputBuffer,unsigned long framesPerBuffer) {
	lock.lock();
	try {
		float *out = (float *)outputBuffer;
		if(watermark>=framesPerBuffer) {
			std::copy(buffer.begin(),buffer.end(),out);
			watermark=0;
		}
	}
	catch(...) {}
	lock.unlock();
}

void AUNonBlockingStream::write(const std::vector<float> &vec) {
	if(active) {
		lock.lock();
		try {
			std::copy(vec.begin(),vec.end(),buffer.begin()+watermark);
			watermark+=vec.size();
		}
		catch(...) {}
		lock.unlock();
	}
}



} /* namespace audio */
