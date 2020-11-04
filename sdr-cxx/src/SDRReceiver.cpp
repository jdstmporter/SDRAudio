/*
 * SDRReceiver.cpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#include "SDRReceiver.hpp"
#include <SoapySDR/Formats.hpp>


namespace sdr {





const unsigned long SDRReceiver::DECIMATION_FACTOR=32;
const float SDRReceiver::AUDIO_RATE=4.8e4;
const unsigned long SDRReceiver::AUDIO_BLOCK_SIZE = 64;
const float SDRReceiver::RF_RATE = SDRReceiver::AUDIO_RATE * (float)SDRReceiver::DECIMATION_FACTOR;
const unsigned long SDRReceiver::RF_BLOCK_SIZE = SDRReceiver::AUDIO_BLOCK_SIZE * SDRReceiver::DECIMATION_FACTOR;


SDRReceiver::SDRReceiver(const unsigned channel_,const float frequency_,
		const float gain_) :
		channel(channel_), bandwidth(-1), frequency(frequency_), rxGain(gain_), stream(nullptr),
		decimator(DECIMATION_FACTOR,AUDIO_BLOCK_SIZE) {

	rfBuffer = new cx_t[RF_BLOCK_SIZE];
	audioBuffer = new cx_t[AUDIO_BLOCK_SIZE];

	rx = SoapySDR::Device::make("driver=rtlsdr");
	rx->setSampleRate(SOAPY_SDR_RX,channel,RF_RATE);
	this->update();
}

SDRReceiver::~SDRReceiver() {
	if(rfBuffer) delete[] rfBuffer;
	if(audioBuffer) delete[] audioBuffer;
	if(rx != nullptr) {
		if(stream != nullptr) {
			rx->deactivateStream(stream);
			rx->closeStream(stream);
			stream=nullptr;
		}
		SoapySDR::Device::unmake(rx);
		rx=nullptr;
	}
}

void SDRReceiver::update() {
	bool shouldReconnect = stream!=nullptr;
	disconnect();

	rx->setFrequency(SOAPY_SDR_RX,channel,frequency);
	if(bandwidth>0) rx->setBandwidth(SOAPY_SDR_RX,channel,bandwidth);
	rx->setGain(SOAPY_SDR_RX,channel,rxGain);
	if(shouldReconnect) connect();
}

void SDRReceiver::connect() {
	if(stream!=nullptr) disconnect();
	stream=rx->setupStream(SOAPY_SDR_RX,SOAPY_SDR_CF32);
	rx->activateStream(stream);
}

void SDRReceiver::disconnect() {
	if(stream!=nullptr) {
		rx->deactivateStream(stream);
		rx->closeStream(stream);
		stream=nullptr;
	}
}


void SDRReceiver::setGain(const float gain_) {
	rxGain=gain_;
	update();
}

void SDRReceiver::tune(const float frequency_,const float bandwidth_) {

	frequency=frequency_;
	bandwidth=std::max(bandwidth,bandwidth_);
	update();

}

SDRData SDRReceiver::load() {
	int flags;
	long long ns;

	//void *const* buffs = &(void *)buffer;
	auto buffs = (void *const *)&rfBuffer;
		int out=rx->readStream(stream,buffs,RF_BLOCK_SIZE,flags,ns);
		if(out<0) throw SDRError(out,flags);
		decimator(rfBuffer);
		std::copy(decimator.begin(),decimator.end(),audioBuffer);

	return SDRData(audioBuffer,AUDIO_BLOCK_SIZE,flags,ns);
}



} /* namespace sdr */

std::ostream & operator<<(std::ostream &o,const sdr::SDRData &f) {
	o << "RX " << f.size() << " samples at time " << std::scientific
			<< f.timestamp/1.0e9 << std::defaultfloat
			<< "; flags: " << f.flags;
	return o;
}
