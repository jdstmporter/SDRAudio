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
#include "AUBasicStream.hpp"
#include <boost/circular_buffer.hpp>

namespace audio {




class AUBlockingStream : public AUBasicStream {

public:
	AUBlockingStream(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters())
		: AUBasicStream(device,params) {};
	AUBlockingStream(const AUStreamParameters &params)
		: AUBlockingStream(AUDevice(),params) {};
	AUBlockingStream()
		: AUBlockingStream(AUDevice(),AUStreamParameters()) {}
	virtual ~AUBlockingStream() = default;

	virtual void write(const std::vector<float> &vec);
};

class AUNonBlockingStream : public AUBasicStream {
private:
	boost::circular_buffer<float> buffer;
	std::mutex lock;

public:
	AUNonBlockingStream(const AUDevice & device,const AUStreamParameters &params = AUStreamParameters())
		: AUBasicStream(device,params), buffer(4096), lock() {};
	AUNonBlockingStream(const AUStreamParameters &params)
		: AUNonBlockingStream(AUDevice(),params) {};
	AUNonBlockingStream()
		: AUNonBlockingStream(AUDevice(),AUStreamParameters()) {}
	virtual ~AUNonBlockingStream() = default;

	virtual void open();
	virtual void write(const std::vector<float> &vec);
	void getNextBlock(void *outputBuffer,unsigned long framesPerBuffer);



};
} /* namespace audio */

#endif /* AUSTREAM_HPP_ */
