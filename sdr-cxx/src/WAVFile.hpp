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


namespace wav {

class WAVFileError : public std::exception {
private:
	int code;
	std::string message;

public:
	WAVFileError(const int code_ = 0,const std::string &message_ = "") :
		std::exception(), code(code_), message(message_) {}
	WAVFileError(const SndfileHandle &handle) :
		WAVFileError(handle.error(),handle.strError()) {};
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
	enum class Format {
		REAL,
		COMPLEX
	};
private:
	SndfileHandle handle;
	size_t bufferSize;
	float *buffer;


	void check() {
		if(!handle) throw WAVFileError(-1,"File not open");
		auto e = WAVFileError(handle);
		if(e) throw e;
	}
public:



	WAVFile(const std::string &path,const Mode &mode = Mode::READ_ONLY,
			const Format &format = Format::REAL, const int sampleRate = 48000,
			const unsigned bufferSize = 32);
	virtual ~WAVFile() ;

	operator bool () const { return handle; }


	size_t read(float *buffer);
	size_t read(cx_t *buffer);

	void write(float *begin,float *end);
	void write(cx_t *begin,cx_t *end);
	void flush();







};

} /* namespace wav */

#endif /* WAVFILE_HPP_ */
