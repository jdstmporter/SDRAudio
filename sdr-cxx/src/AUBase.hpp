/*
 * AUBase.hpp
 *
 *  Created on: 6 Nov 2020
 *      Author: julianporter
 */

#ifndef AUBASE_HPP_
#define AUBASE_HPP_

#include <portaudio.h>
#include <string>
#include <iostream>
#include <exception>
#include <vector>

namespace audio {

using time_t = PaTime;
using flags_t = PaStreamCallbackFlags;

class AUError : public std::exception {
public:
	using error_t = PaError;
private:
	error_t error;
	std::string message;
public:
	AUError(const error_t error_) : error(error_), message(Pa_GetErrorText(error_)) {}
	AUError(const std::string &message_) : error(paInternalError), message(message_) {}
	virtual ~AUError() = default;
	AUError(const AUError &) = default;

	virtual const char *what() const noexcept { return message.c_str(); }
	operator error_t() { return error; }

	static void check(error_t error) {
		if(error!=paNoError) throw AUError(error);
	}
};

enum class AUSampleType : PaSampleFormat {
	Int8 = paInt8,
	UInt8 = paUInt8,
	Int16 = paInt16,
	Int24 = paInt24,
	Int32 = paInt32,
	Float = paFloat32
};

}





#endif /* AUBASE_HPP_ */
