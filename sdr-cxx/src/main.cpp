/*
 * main.cpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#include "SDRReceiver.hpp"
#include "IOAudio.hpp"
#include "AUStream.hpp"
#include "WAVFile.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <cmath>

int main(const int argc,const char **argv) {
	try {
		audio::AUAudio au;
		au.enumerate();
		for(auto it = au.begin();it != au.end(); it++) {
			std::cout << *it << std::endl;
		}
		std::cout << "Default device has index " << audio::AUDevice().index() << std::endl;
		audio::AUNonBlockingStream stream;


		auto centreFrequency = std::stof(argv[1])*1.0e6;
		auto gain = std::stof(argv[2]);
		std::string filename(argv[3]);
		unsigned bufferSize=2*sdr::SDRReceiver::AUDIO_BLOCK_SIZE;

		wav::WAVFile wavfile(filename,wav::WAVFile::Mode::WRITE_ONLY,
							 (int)sdr::SDRReceiver::AUDIO_RATE,2);

		sdr::SDRReceiver rx(0,centreFrequency,10.0);
		rx.connect();

		std::vector<float> buffer(bufferSize,0);


		//stream.start();
		//while(true) {
		for(auto i=0;i<10000;i++) {
			try {
				auto response = rx.load();
				//std::cout << response << std::endl;
				auto it=buffer.begin();
				for(auto it2=response.begin();it2!=response.end();it2++) {
					auto value=(*it2)*gain;
					auto r = value.real();
					auto i = value.imag();
					*(it++)=(fabs(r)>0.2) ?  0 : r;
					*(it++)=(fabs(i)>0.2) ?  0 : i;
				}
				auto mima=std::minmax_element(buffer.begin(),buffer.end());
				std::cout << "Min " << *mima.first << " Max " << *mima.second << std::endl;
				wavfile.write(buffer);
				//stream.write(buffer);
			}
			catch(sdr::SDRError &e) {
				std::cerr << ": " << e.what() << std::endl;
			}
			catch(std::exception &e) {
				std::cerr << ": " << e.what() << std::endl;
			}
		}
		stream.stop();
		//wavfile.flush();
		rx.disconnect();
	}
	catch(std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}

}




