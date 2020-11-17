/*
 * SDRReceiver.cpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#include "SDRReceiver.hpp"
#include <SoapySDR/Formats.hpp>


namespace sdr {






SDRReceiver::SDRReceiver(const unsigned channel_,const float frequency_,
		const float gain_) :
		channel(channel_), bandwidth(-1), frequency(frequency_), rxGain(gain_), stream(nullptr) {

	buffer = new cx_t[SDRConstants::RF_BLOCK_SIZE];

	rx = SoapySDR::Device::make("driver=rtlsdr");
	rx->setSampleRate(SOAPY_SDR_RX,channel,SDRConstants::RF_RATE);
	this->update();
}

SDRReceiver::~SDRReceiver() {
	if(buffer != nullptr) delete[] buffer;
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
	auto buffs = (void *const *)&buffer;
		int out=rx->readStream(stream,buffs,SDRConstants::RF_BLOCK_SIZE,flags,ns);
		if(out<0) throw SDRError(out,flags);
	return SDRData(buffer,SDRConstants::RF_BLOCK_SIZE,flags,ns);
}



} /* namespace sdr */

std::ostream & operator<<(std::ostream &o,const sdr::SDRData &f) {
	o << "RX " << f.size() << " samples at time " << std::scientific
			<< f.timestamp/1.0e9 << std::defaultfloat
			<< "; flags: " << f.flags;
	return o;
}
