/*
 * IOAudio.cpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#include "IOAudio.hpp"

namespace audio {





AUAudio::AUAudio() : initialised(false)  {
	AUError::check(Pa_Initialize());
	initialised=true;

}

AUAudio::~AUAudio() {
	try {
		if(initialised) Pa_Terminate();
		initialised=false;
	}
	catch(...) {}
}

void AUAudio::enumerate() {
	auto nDevices=Pa_GetDeviceCount();
	if(nDevices<0) throw AUError("Cannot enumerate devices");
	devices.clear();
	for(int i=0;i<nDevices;i++) {
		devices.push_back(AUDevice(i));
	}

}






} /* namespace audio */
