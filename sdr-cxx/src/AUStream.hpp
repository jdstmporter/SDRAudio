/*
 * AUStream.hpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#ifndef AUSTREAM_HPP_
#define AUSTREAM_HPP_

#include <portaudio.h>
#include <string>
#include "AUBase.hpp"
#include "AUDevice.hpp"

namespace audio {

struct AUStreamParameters {
	using sample_t = PaSampleFormat;

	unsigned nChannels;
	AUSampleType format;
	double rate;
	unsigned bufferSize;

	AUStreamParameters(const unsigned nChannels_=1,const AUSampleType format_=AUSampleType::Float,
			const double rate_=4.8e4,const unsigned bufferSize_=1024) :
				nChannels(nChannels_), format(format_), rate(rate_), bufferSize(bufferSize_) {};
	virtual ~AUStreamParameters()=default;

	unsigned long sampleSize() const;
	unsigned long frameSize() const { return sampleSize()*nChannels; }
	sample_t sampleFormat() const { return static_cast<sample_t>(format); }
};

class AUStream {
private:
	PaStream *stream;
	bool active;
	unsigned long fpb;

public:
	AUStream(const AUStreamParameters &params = AUStreamParameters());
	AUStream(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters());
	virtual ~AUStream();

	void start();
	void stop();
	void forceStop();

	void write(float *buffer);
};

} /* namespace audio */

#endif /* AUSTREAM_HPP_ */
