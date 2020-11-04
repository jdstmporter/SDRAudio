/*
 * SoapyError.hpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRERROR_HPP_
#define SDRERROR_HPP_


#include "base.hpp"
#include <SoapySDR/Errors.h>
#include "SDRFlags.hpp"

namespace sdr {

class SDRError: public std::exception {
private:
	int code;
	SDRFlags flags;
	std::string message;
	std::string text;
public:
	SDRError(const int code_,const unsigned flags_) noexcept : std::exception(), code(code_), flags(SDRFlags(flags_)) {
		message = std::string(SoapySDR_errToStr(code));
		std::stringstream s;
		s << "SDR error " << code << " (" << message << "), Flags: " << flags;
		text = s.str();
	}
	virtual ~SDRError()  = default;
	SDRError & operator=(const SDRError &) = default;
	SDRError(const SDRError &) = default;



	virtual const char *what() const noexcept {
		return text.c_str();
	}
};

} /* namespace sdr */

#endif /* SDRERROR_HPP_ */
