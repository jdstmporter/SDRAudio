/*
 * SDRReceiver.hpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRRECEIVER_HPP_
#define SDRRECEIVER_HPP_

#include "base.hpp"
#include <SoapySDR/Device.hpp>

#include "SDRFlags.hpp"
#include "SDRError.hpp"

namespace sdr {

using cx_t = std::complex<float>;




struct SDRData {
	using iterator = cx_t *;
	using const_iterator = const cx_t *;

	cx_t *buffer;
	SDRFlags flags;
	long long timestamp;
	unsigned n;

	SDRData(cx_t *b,const unsigned n_,const int flags_,const long long ns) :
		buffer(b), flags(SDRFlags(flags_)), timestamp(ns), n(n_) {}
	virtual ~SDRData() = default;
	SDRData(const SDRData &) = default;
	SDRData(SDRData &&) = default;
	SDRData & operator=(const SDRData &) = default;
	SDRData & operator=(SDRData &&) = default;

	iterator begin() { return buffer; }
	iterator end() { return buffer+n; }
	const_iterator cbegin() const { return buffer; }
	const_iterator cend() const { return buffer+n; }

	unsigned size() const { return n; }

	operator CXData() {
		return CXData(buffer,n);
	}

};

class SDRReceiver {
private:
	size_t bufferSize;
	float rate;
	unsigned channel;
	float bandwidth;
	float frequency;

	float rxGain;

	cx_t *buffer;
	SoapySDR::Device *rx;
	SoapySDR::Stream *stream;

	void update();
public:
	SDRReceiver(const size_t bufferSize_ = 32768,
				const float rate = 1536000.f,
				const unsigned channel_=0,
			    const float frequency_=433.0e6,
				const float gain_ = 0.0);
	virtual ~SDRReceiver();

	void connect();
	void disconnect();
	void tune(const float frequency,const float bandwidth=-1);
	void setGain(const float gain_);
	SDRData load();




};

} /* namespace sdr */

std::ostream & operator<<(std::ostream &o,const sdr::SDRData &f);

#endif /* SDRRECEIVER_HPP_ */
