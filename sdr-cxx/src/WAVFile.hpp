/*
 * WAVFile.hpp
 *
 *  Created on: 5 Nov 2020
 *      Author: julianporter
 */

#ifndef WAVFILE_HPP_
#define WAVFILE_HPP_

#include <sndfile.hh>
#include <string>
#include <sstream>
#include <exception>
#include <complex>
#include <vector>


namespace wav {

class WAVFileError : public std::exception {
private:
	int code;
	std::string message;

public:
	WAVFileError(const int code_ = 0,const std::string &message_ = "") :
		std::exception(), code(code_), message(message_) {}
	WAVFileError(SNDFILE *handle) :
		WAVFileError(sf_error(handle),sf_strerror(handle)) {};
	WAVFileError(const WAVFileError &) = default;
	virtual ~WAVFileError() = default;

	operator bool () const { return code != 0; }
	virtual const char * what() const noexcept { return message.c_str(); }
	int error() const { return code; }


};

class WAVFile {
public:
	using size_t = sf_count_t;
	using cx_t = std::complex<float>;

	enum class Mode : int {
		READ_ONLY = SFM_READ,
		WRITE_ONLY = SFM_WRITE,
		READ_WRITE = SFM_RDWR
	};
private:
	unsigned nChannels;
	SNDFILE *handle;


	void check() {
		if(!handle) throw WAVFileError(-1,"File not open");
		auto e = WAVFileError(handle);
		if(e) throw e;
	}
	size_t size2Frames(unsigned n) const { return (size_t)(n/nChannels); }
public:



	WAVFile(const std::string &path,
			const Mode &mode = Mode::WRITE_ONLY,
			const int sampleRate = 48000,
			const unsigned nChannels = 2);
	virtual ~WAVFile() ;

	operator bool () const { return handle; }


	size_t read(float *buffer,const unsigned n);

	void write(float *begin,float *end);
	void write(const std::vector<float> &);







};

} /* namespace wav */

#endif /* WAVFILE_HPP_ */
