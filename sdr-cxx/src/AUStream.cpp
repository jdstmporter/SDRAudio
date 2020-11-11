/*
 * AUStream.cpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#include "AUStream.hpp"
#include <algorithm>

namespace audio {

static int callback(const void *inputBuffer,
					void *outputBuffer,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* timeInfo,
					PaStreamCallbackFlags statusFlags,
					void *userData) {
	auto me = (AUNonBlockingStream *)userData;

	me->getNextBlock(outputBuffer,framesPerBuffer);
	return 0;
}


void AUBlockingStream::write(const std::vector<float> &vec) {
	if(stream!=nullptr) {
		AUError::check(Pa_WriteStream(stream,vec.data(),vec.size()));
	}
}

void AUNonBlockingStream::open() {
	PaStream *stream_;
	PaStreamParameters p=params.streamParameters(device);
	AUError::check(Pa_OpenStream(&stream_,nullptr,&p,params.rate,fpb,paNoFlag,
			callback,(void *)this));
	stream=stream_;
}

void AUNonBlockingStream::getNextBlock(void *outputBuffer,unsigned long framesPerBuffer) {
	//lock.lock();
	try {

		if(buffer.size()>=framesPerBuffer) {
			float *out = (float *)outputBuffer;
			std::copy(buffer.begin(),buffer.begin()+framesPerBuffer,out);
			buffer.erase_begin(framesPerBuffer);
		}
		else {
			std::cout << "MISS" << std::endl;
		}
	}
	catch(...) {}
	//lock.unlock();
}

void AUNonBlockingStream::write(const std::vector<float> &vec) {
	if(stream!=nullptr) {
		//lock.lock();
		try {
			buffer.insert(buffer.end(),vec.begin(),vec.end());
			//std::cout << "inserted " << vec.size() << " giving " << buffer.size() << std::endl;
		}
		catch(...) {}
		//lock.unlock();
	}
}



} /* namespace audio */
