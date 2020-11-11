/*
 * SDRFlags.cpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#include "SDRFlags.hpp"
#include <initializer_list>

namespace sdr {


std::pair<const unsigned,std::string> _p(const unsigned u,const char *c) { return std::make_pair(u,std::string(c)); }

const std::map<unsigned,std::string> SDRFlags::flagNames ({
		_p(SOAPY_SDR_END_BURST, "End burst"),
		_p(SOAPY_SDR_HAS_TIME, "Has valid timestamp"),
		_p(SOAPY_SDR_END_ABRUPT, "Stream terminated prematurely"),
		_p(SOAPY_SDR_ONE_PACKET, "Single packet"),
		_p(SOAPY_SDR_MORE_FRAGMENTS, "More fragments to come"),
		_p(SOAPY_SDR_WAIT_TRIGGER, "Wait for external trigger")
});

SDRFlags::SDRFlags(const int flags_) : flag(flags_), flags()  {
	unsigned matched=0;
	for(auto it = flagNames.begin();it!=flagNames.end();it++) {
		auto f = it->first;
		if((f &flag) != 0) {
			matched |= f;
			flags.push_back(f);
		}
	}
	anomaly = matched ^ flags_;
}

std::string SDRFlags::toString() const {
	std::stringstream s;
	s << "(0x" << std::hex << flag << std::dec << ") ";
	for(auto it=flags.begin();it!=flags.end();it++) {
		s << flagNames.at(*it) << "; ";
	}
	if(anomaly!=0) {
		s << "anomalous flags 0x" << std::hex << anomaly;
	}
	return s.str();
}



} /* namespace sdr */

std::ostream & operator<<(std::ostream &o,const sdr::SDRFlags &f) {
	o  << f.toString();
	return o;
}
