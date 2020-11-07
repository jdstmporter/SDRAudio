/*
 * IOAudio.hpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#ifndef IOAUDIO_HPP_
#define IOAUDIO_HPP_

#include "AUBase.hpp"
#include "AUStream.hpp"

namespace audio {

class AUAudio {
public:
	using iterator=std::vector<AUDevice>::iterator;
	using const_iterator=std::vector<AUDevice>::const_iterator;
private:
	bool initialised;
	std::vector<AUDevice> devices;

public:
	AUAudio();
	virtual ~AUAudio();

	void enumerate();
	AUDevice defaultDevice() const { return AUDevice(); }

	AUDevice operator[](const unsigned n) const { return devices.at(n); }
	unsigned size() const { return devices.size(); }

	iterator begin() { return devices.begin(); }
	iterator end() { return devices.end(); }
	const_iterator cbegin() const { return devices.cbegin(); }
	const_iterator cend() const { return devices.cend(); }


};

} /* namespace audio */

#endif /* IOAUDIO_HPP_ */
