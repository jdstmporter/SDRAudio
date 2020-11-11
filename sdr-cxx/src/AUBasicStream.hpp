/*
 * AUBasicStream.hpp
 *
 *  Created on: 10 Nov 2020
 *      Author: julianporter
 */

#ifndef AUBASICSTREAM_HPP_
#define AUBASICSTREAM_HPP_

#include <portaudio.h>
#include <string>
#include <mutex>
#include "AUStreamParameters.hpp"

namespace audio {



class AUBasicStream {
protected:
	PaStream *stream;
	AUStreamParameters params;
	AUDevice device;
	unsigned long fpb;
	unsigned bufferSize;

	virtual void open();

public:

	AUBasicStream(const AUDevice & device_ = AUDevice(),
				  const AUStreamParameters &params_ = AUStreamParameters())
		: stream(nullptr), params(params_), device(device_),
		  fpb(params.bufferSize/params.nChannels),
		  bufferSize(params.bufferSize) {}
	AUBasicStream(const AUStreamParameters &params = AUStreamParameters())
		: AUBasicStream(AUDevice(),params) {};
	virtual ~AUBasicStream();


	virtual void start();
	virtual void stop();
	virtual void forceStop();

	virtual void write(const std::vector<float> &vec) =0;
	unsigned size() const { return bufferSize; }

	operator bool() { return stream!=nullptr; }

};


} /* namespace audio */

#endif /* AUBASICSTREAM_HPP_ */
