/*
 * SDRFlags.hpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRFLAGS_HPP_
#define SDRFLAGS_HPP_

#include "base.hpp"
#include <SoapySDR/Constants.h>
#include <map>
#include <vector>


namespace sdr {

class SDRFlags {
private:
	static const std::map<unsigned,std::string> flagNames;
	unsigned flag;
	std::vector<unsigned> flags;
	unsigned anomaly;

public:
	using iterator = std::vector<unsigned>::iterator;
	using const_iterator = std::vector<unsigned>::const_iterator;

	SDRFlags(const unsigned flags_=0);
	virtual ~SDRFlags() = default;
	SDRFlags(const SDRFlags &other) = default;
	SDRFlags(SDRFlags &&other) = default;
	SDRFlags& operator=(const SDRFlags &other) = default;
	SDRFlags& operator=(SDRFlags &&other) = default;

	std::string toString() const;
	static std::string nameOf(const unsigned f) { return flagNames.at(f); }

	iterator begin() { return flags.begin(); }
	iterator end() { return flags.end(); }
	const_iterator cbegin() const { return flags.cbegin(); }
	const_iterator cend() const { return flags.cend(); }

	unsigned anomalousFlags() const { return anomaly; }

};

} /* namespace sdr */


std::ostream & operator<<(std::ostream &o,const sdr::SDRFlags &f);


#endif /* SDRFLAGS_HPP_ */
