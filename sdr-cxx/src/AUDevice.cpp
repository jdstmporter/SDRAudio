/*
 * AUDevice.cpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#include "AUDevice.hpp"

#include <sstream>

namespace audio {

std::string AUDevice::toString() const {
	std::stringstream s;
	s << "Device " << idx << ": '" << name << "' Max input channels: "
      << nInChannels << " output channels: " << nOutChannels;
	return s.str();
}

}

std::ostream &operator<<(std::ostream &o,const audio::AUDevice &d) {
	o << d.toString();
	return o;
}
