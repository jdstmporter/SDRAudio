/*
 * AUBasicStream.cpp
 *
 *  Created on: 10 Nov 2020
 *      Author: julianporter
 */

#include "AUBasicStream.hpp"

namespace audio {


AUBasicStream::~AUBasicStream() {
	if(stream!=nullptr) {
		try {
			Pa_CloseStream(stream);
			stream=nullptr;
		}
		catch(...) {}
	}
}

void AUBasicStream::open() {
	PaStream *stream_;
	PaStreamParameters p=params.streamParameters(device);
	AUError::check(Pa_OpenStream(&stream_,nullptr,&p,params.rate,fpb,paNoFlag,nullptr,nullptr));
	stream=stream_;
}

void AUBasicStream::start() {
	if(stream==nullptr) {
		open();
		AUError::check(Pa_StartStream(stream));
	}
}

void AUBasicStream::stop() {
	if(stream!=nullptr) {
		AUError::check(Pa_StopStream(stream));
		stream=nullptr;
	}
}

void AUBasicStream::forceStop() {
	if(stream!=nullptr) {
		AUError::check(Pa_AbortStream(stream));
		stream=nullptr;
	}
}


} /* namespace audio */
