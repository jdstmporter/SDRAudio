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
#include <liquid/liquid.h>
#include "SDRFlags.hpp"
#include "SDRError.hpp"
#include "SDRDecimator.hpp"

namespace sdr {

using cx_t = std::complex<float>;
using lcx_t = liquid_float_complex;



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

};

class SDRReceiver {
private:


	const static float RF_RATE;
	const static unsigned long DECIMATOR_LENGTH;

	const static unsigned long RF_BLOCK_SIZE;
	unsigned channel;
	float bandwidth;
	float frequency;

	float rxGain;
	cx_t *rfBuffer;
	cx_t *audioBuffer;
	SoapySDR::Device *rx;
	SoapySDR::Stream *stream;
	SDRDecimator decimator;

	void update();
public:
	const static unsigned long AUDIO_BLOCK_SIZE;
	const static float AUDIO_RATE;
	const static unsigned long DECIMATION_FACTOR;

	SDRReceiver(const unsigned channel_=0,
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
