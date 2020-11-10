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
#include <mutex>
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

	AUStreamParameters(const unsigned nChannels_=2,const AUSampleType format_=AUSampleType::Float,
			const double rate_=4.8e4,const unsigned bufferSize_=1024) :
				nChannels(nChannels_), format(format_), rate(rate_), bufferSize(bufferSize_) {};
	virtual ~AUStreamParameters()=default;

	unsigned long sampleSize() const;
	unsigned long frameSize() const { return sampleSize()*nChannels; }
	sample_t sampleFormat() const { return static_cast<sample_t>(format); }

	params_t streamParameters(const AUDevice &) const;
};

class AUStreamBase {
private:
	bool active;
	unsigned long fpb;
	unsigned bufferSize;
protected:
	PaStream *stream;
	AUStreamParameters params;
	AUDevice device;



public:


	AUStreamBase(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters(),);
	AUStreamBase(const AUStreamParameters &params = AUStreamParameters()) : AUStreamBase(AUDevice(),params) {};
	virtual ~AUStreamBase();

	virtual void open();
	virtual void start();
	virtual void stop();
	virtual void forceStop();

	virtual void write(const std::vector<float> &vec) =0;
	unsigned size() const { return bufferSize; }

};

class AUStream : public AUStreamBase {

public:
	AUStream(const AUStreamParameters &params = AUStreamParameters()) : AUStreamBase(params) {};
	AUStream(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters()) :
		AUStreamBase(device,params) {};
	virtual ~AUStream() = default;

	virtual void write(const std::vector<float> &vec);
};

class AUNonBlockingStream : public AUStreamBase {
private:
	std::vector<float> buffer;
	unsigned watermark;
	std::mutex lock;

protected:
	void getNextBlock(void *outputBuffer,unsigned long framesPerBuffer);

public:
	AUNonBlockingStream(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters())  :
		AUStreamBase(device,params), buffer(1024,0), watermark(0), lock() {};
	AUNonBlockingStream(const AUStreamParameters &params = AUStreamParameters()) : AUNonBlockingStream(AUDevice(),params) {};
	virtual ~AUNonBlockingStream() = default;

	virtual void open();
	virtual void write(const std::vector<float> &vec);



};
} /* namespace audio */

#endif /* AUSTREAM_HPP_ */
