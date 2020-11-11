/*
 * WAVFile.cpp
 *
 *  Created on: 5 Nov 2020
 *      Author: julianporter
 */

#include "WAVFile.hpp"
#include <algorithm>

namespace wav {

WAVFile::WAVFile(const std::string &path_,const Mode &mode_,
		const int sampleRate_, const unsigned nChannels_) :
			nChannels(nChannels_) {

		SF_INFO info;
		info.samplerate = sampleRate_;
		info.channels = (int)nChannels;
		info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;

		if(!sf_format_check(&info)) {
			throw  WAVFileError(0,"Bad sound file format specifier");
		}

		handle = sf_open(path_.c_str(),static_cast<int>(mode_),&info);
		check();
}

WAVFile::~WAVFile() {
	if(handle) {
		sf_write_sync(handle);
		sf_close(handle);
		handle=nullptr;
	}
}

WAVFile::size_t WAVFile::read(float *buffer,const unsigned n) {
	auto out=sf_readf_float(handle,buffer,n);
	check();
	return out;
}



void WAVFile::write(float *begin,float *end) {
	sf_writef_float(handle,begin,size2Frames(end-begin));
	check();
}
void WAVFile::write(const std::vector<float> &vec) {
	sf_writef_float(handle,vec.data(),size2Frames(vec.size()));
	check();
}






} /* namespace wav */
