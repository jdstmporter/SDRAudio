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

int main(const int argc,const char **argv) {
	try {
//		audio::AUAudio au;
//		au.enumerate();
//		for(auto it = au.begin();it != au.end(); it++) {
//			std::cout << *it << std::endl;
//		}
//		std::cout << "Default device has index " << audio::AUDevice().index() << std::endl;
//		audio::AUStream stream(au[1]);
		//unsigned sz=stream.size();

		auto centreFrequency = std::stof(argv[1])*1.0e6;

		wav::WAVFile wavfile("fred.wav",wav::WAVFile::Mode::WRITE_ONLY,
				wav::WAVFile::Format::REAL,48000,64);

		sdr::SDRReceiver rx(0,centreFrequency,10.0);
		rx.connect();

		std::vector<float> buffer(64,0);


		auto begin=buffer.begin();
		unsigned offset=0;
		//while(true) {
		for(auto i=0;i<10000;i++) {
			try {
				auto response = rx.load();
				std::cout << response << std::endl;
				std::transform(response.begin(),response.end(),buffer.begin(),
										[](auto cx) {
					float sgn = (std::arg(cx)>0.0) ? 1.0 : -1.0;
					return 100*std::abs(cx)*sgn; });
				auto mima=std::minmax_element(buffer.begin(),buffer.end());
				std::cout << "Min " << *mima.first << " Max " << *mima.second << std::endl;
				wavfile.write(buffer);
//				offset+=response.size();
//				std::cout << "Output offset is " << offset << " of " << sz << std::endl;
//				if(offset>=sz) {
//					stream.write(buffer);
//					offset=0;
//				}
			}
			catch(sdr::SDRError &e) {
				std::cerr << ": " << e.what() << std::endl;
			}
			catch(std::exception &e) {
				std::cerr << ": " << e.what() << std::endl;
			}
		}
		//stream.stop();
		wavfile.flush();
		rx.disconnect();
	}
	catch(std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}

}




