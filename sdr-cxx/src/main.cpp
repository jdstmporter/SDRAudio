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
#include <kfr/io.hpp>
#include "SDRDecimator.hpp"

const unsigned long factor = 32;
const unsigned long long outFreq = 48000;
constexpr unsigned long long inFreq = outFreq*factor;
const float freq = 40000;
constexpr float phaseFactor = 2*sdr::PI/(float)inFreq;

int main(const int argc,const char **argv) {
	try {
		//udio::AUAudio au;
		//au.enumerate();
		//for(auto it = au.begin();it != au.end(); it++) {
		//	std::cout << *it << std::endl;
		//}
		//std::cout << "Default device has index " << audio::AUDevice().index() << std::endl;

		//auto centreFrequency = std::stof(argv[1])*1.0e6;
		//auto gain = std::stof(argv[2]);
		//std::string filename(argv[3]);


		std::cout << "Make decimator " << std::endl;
		sdr::SDRDecimator decimator(48000,96000,1024);
		std::cout << "Made decimator " << std::endl;

		std::vector<sdr::cx_t> sine(1024*factor,sdr::cx_zero);
		std::vector<float> ins(2048*factor,0);
		std::vector<float> outs(2048,0);


		std::cout << "set up vectors" << std::endl;

		wav::WAVFile inpfile("ijinput.wav",wav::WAVFile::Mode::WRITE_ONLY,
											 inFreq,2);
		wav::WAVFile wavfile("joe2.wav",wav::WAVFile::Mode::WRITE_ONLY,
									 outFreq,2);

		std::cout << "opened file " << std::endl;



		float p=0;
		float phase = freq*phaseFactor;
		for(auto j=0;j<200;j++) {

			std::cout << "Batch " << j << " freq " << freq << std::endl;
			std::cout << "  Generating" << std::endl;
			for(auto it=sine.begin();it!=sine.end();it++) {
				*it = sdr::cx_t(cos(p),sin(p));
				p+=phase;
				while (p>2*sdr::PI) p-=2*sdr::PI;
			}
			sdr::CXData sineRI(sine);
			auto iit = ins.begin();
						for(auto i=0;i<2048*factor;i++) {
							(*iit++) = sineRI[i];
						}
						inpfile.write(ins);
			std::cout << "  Decimating" << std::endl;
			decimator(sine);
			std::cout << "  Converting " << std::endl;
			auto oit = outs.begin();
			for(auto i=0;i<2048;i++) {
				(*oit++) = 0.5*decimator[i];
			}
			std::cout << "  Writing" << std::endl;

			auto mima=std::minmax_element(outs.begin(),outs.end());
			std::cout << "  Min " << *mima.first << " Max " << *mima.second << std::endl;


			wavfile.write(outs);
		}


/*

		audio::AUNonBlockingStream stream;


		auto centreFrequency = std::stof(argv[1])*1.0e6;
		auto gain = std::stof(argv[2]);
		std::string filename(argv[3]);
		unsigned bufferSize=2*sdr::SDRReceiver::AUDIO_BLOCK_SIZE;

		wav::WAVFile wavfile(filename,wav::WAVFile::Mode::WRITE_ONLY,
							 (int)sdr::SDRReceiver::AUDIO_RATE,1);

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
					auto r = std::real(value);
					auto i = std::imag(value);
					*(it++)=r;
					*(it++)=i;
				}
				//auto mima=std::minmax_element(buffer.begin(),buffer.end());
				//std::cout << "Min " << *mima.first << " Max " << *mima.second << std::endl;
				wavfile.write(buffer);
				//stream.write(buffer);
			}
			catch(sdr::SDRError &e) {
				std::cerr << "Error : " << e.what() << std::endl;
			}
			catch(std::exception &e) {
				std::cerr << "Error : " << e.what() << std::endl;
			}
		}
		stream.stop();
		//wavfile.flush();

rx.disconnect();
		 */

	}
	catch(std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}

}




