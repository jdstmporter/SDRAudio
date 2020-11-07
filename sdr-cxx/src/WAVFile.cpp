/*
 * WAVFile.cpp
 *
 *  Created on: 5 Nov 2020
 *      Author: julianporter
 */

#include "WAVFile.hpp"
#include <algorithm>

namespace wav {

WAVFile::WAVFile(const std::string &path_,const Mode &mode_,const Format &format,
		const int sampleRate_, const unsigned bufferSize_)  {
		int n = (format == Format::REAL) ? 1 : 2;
		bufferSize=(size_t)(bufferSize_*n);
		buffer = new float[bufferSize];
		handle = SndfileHandle(path_,static_cast<int>(mode_),
						               SF_FORMAT_WAV | SF_FORMAT_FLOAT,n,sampleRate_);
		check();
}

WAVFile::~WAVFile() {
	if(handle) flush();
	delete [] buffer;
}

WAVFile::size_t WAVFile::read(float *buffer) {
	auto out=handle.readf(buffer,bufferSize);
	check();
	return out;
}

WAVFile::size_t WAVFile::read(cx_t *cbuffer) {
	auto out=handle.readf(buffer,(size_t)bufferSize)/2;
	check();
	for(auto i=0;i<out;i++) {
		cbuffer[i]=cx_t(buffer[2*i],buffer[2*i+1]);
	}
	return out;
}

void WAVFile::write(float *begin,float *end) {
	auto n = (size_t)(end-begin);
	handle.writef(begin,n);
	check();
}

void WAVFile::write(cx_t *begin,cx_t *end) {
	auto n = std::min<size_t>(bufferSize,(size_t)(end-begin));
	for(size_t i=0;i<n;i++) {
		auto c = *(begin+i);
		buffer[2*i]=c.real();
		buffer[2*i+1] = c.imag();
	}
	handle.writef(buffer,2*n);
	check();
}

void WAVFile::flush() { handle.writeSync(); }



} /* namespace wav */
