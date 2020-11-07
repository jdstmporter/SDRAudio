/*
 * AUDevice.hpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#ifndef AUDEVICE_HPP_
#define AUDEVICE_HPP_

#include "AUBase.hpp"


namespace audio {

class AUDevice {
private:
	int idx;
	std::string name;
	PaHostApiIndex api;
	int nInChannels;
	int nOutChannels;
	double defaultRate;
	time_t lowLatencyOut;
	time_t lowLatencyIn;


public:
	AUDevice(const int index_) {
		idx=index_;
		auto info=Pa_GetDeviceInfo(idx);
		name=std::string(info->name);
		api=info->hostApi;
		nInChannels=info->maxInputChannels;
		nOutChannels=info->maxOutputChannels;
		defaultRate=info->defaultSampleRate;
		lowLatencyOut=info->defaultHighOutputLatency;
		lowLatencyIn=info->defaultLowInputLatency;
	}
	AUDevice() : AUDevice(Pa_GetDefaultOutputDevice()) {}

	int index() const { return idx; }
	time_t latencyOut() const { return lowLatencyOut; }
	std::string toString() const;

};

} /* namespace audio */

std::ostream &operator<<(std::ostream &,const audio::AUDevice &);

#endif /* AUDEVICE_HPP_ */
