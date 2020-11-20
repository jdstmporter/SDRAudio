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


//#define DECIMATE


int main(const int argc,const char **argv) {
	try {

#ifdef DECIMATE
		unsigned long factor = 16;
		float freq = 5000.f;
		if(argc>=3) {
			try { factor = std::stoul(argv[1]); } catch(...) {}
			try { freq = std::stof(argv[2]); } catch(...) {}
		}
		float outFreq = 24000;
		float inFreq = outFreq*factor;
		unsigned long outBufferSize = 1024;
		unsigned long inBufferSize = outBufferSize * factor;


		float phaseFactor = 2*sdr::PI/(float)inFreq;
		float phase = freq*phaseFactor;

		std::cout << "Proceeding as follows:" << std::endl;
		std::cout << "    In rate  = " << inFreq << std::endl;
		std::cout << "    Out rate = " << outFreq << std::endl;
		std::cout << "    Factor   = " << factor << std::endl;
		std::cout << "    Signal   = " << freq << " Hz" << std::endl;


		auto decimator = sdr::Decimator<sdr::SDRSimpleLPFDecimator>(inFreq,inBufferSize,factor);


		std::vector<sdr::cx_t> sine(inBufferSize,sdr::cx_zero);
		std::vector<float> ins(2*inBufferSize,0);
		std::vector<float> outs(2*outBufferSize,0);


		std::cout << "set up vectors" << std::endl;

		wav::WAVFile inpfile("ijinput.wav",wav::WAVFile::Mode::WRITE_ONLY,(int)inFreq,2);
		wav::WAVFile wavfile("joe2.wav",wav::WAVFile::Mode::WRITE_ONLY,(int)outFreq,2);

		std::cout << "opened files " << std::endl;



		float p=0;

		for(auto j=0;j<200;j++) {

			//std::cout << "Batch " << j << " freq " << freq << std::endl;
			//std::cout << "  Generating" << std::endl;
			for(auto it=sine.begin();it!=sine.end();it++) {
				*it = sdr::cx_t(cos(p),sin(p));
				p+=phase;
				while (p>2*sdr::PI) p-=2*sdr::PI;
			}
			sdr::CXData sineRI(sine);
			auto iit = ins.begin();
			for(auto i=0;i<2*inBufferSize;i++) (*iit++) = sineRI[i];
			inpfile.write(ins);

			//std::cout << "  Decimating" << std::endl;
			(*decimator)(sine);


			size_t idx=0;
			for(auto it=outs.begin();it!=outs.end();it++) {
				*it = 0.5*(*decimator)[idx++];
			}


			auto mima=std::minmax_element(outs.begin(),outs.end());
			std::cout << "Batch " << j << "  Min " << *mima.first << " Max " << *mima.second << std::endl;


			wavfile.write(outs);
		}

#else
		unsigned long factor = 64;
		float centreFrequency = 1.518;
		if(argc>=2) {
			try { centreFrequency = std::stof(argv[1])*1.0e6; } catch(...) {}
		}
		float outFreq = 24000;
		float inFreq = outFreq*factor;
		unsigned long outBufferSize = 1024;
		unsigned long inBufferSize = outBufferSize * factor;


				float gain = 10.0;
				std::string filename="joe";
				if(argc>=4) {
					try {
						centreFrequency = std::stof(argv[1])*1.0e6;
						gain = std::stof(argv[2]);
						filename = std::string(argv[3]);
					}
					catch(...) {
						std::cerr << "test <centre freq in MHz> <gain in dB> <filename>" << std::endl;
						return 1;
					}
				}

		audio::AUAudio au;
		au.enumerate();
		for(auto it = au.begin();it != au.end(); it++) {
			std::cout << *it << std::endl;
		}
		std::cout << "Default device has index " << audio::AUDevice().index() << std::endl;

		audio::AUNonBlockingStream stream;

		wav::WAVFile wavfile(filename,wav::WAVFile::Mode::WRITE_ONLY,
				(int)outFreq,1);

		sdr::SDRReceiver rx(inBufferSize,inFreq,0,centreFrequency,gain);
		rx.connect();

		std::vector<float> outs(2*outBufferSize,0);
		auto decimator = sdr::Decimator<sdr::SDRSimpleLPFDecimator>(inFreq,inBufferSize,factor);

		//stream.start();
		//while(true) {
		for(auto i=0;i<10000;i++) {
			try {
				auto response = rx.load();
				(*decimator)(response.buffer);

				size_t idx=0;
				for(auto it=outs.begin();it!=outs.end();it++) {
					*it = 0.5*(*decimator)[idx++];
				}

				wavfile.write(outs);
				auto mima=std::minmax_element(outs.begin(),outs.end());
				std::cout << "  Min " << *mima.first << " Max " << *mima.second << std::endl;
			}
			catch(sdr::SDRError &e) {
				std::cerr << "Error : " << e.what() << std::endl;
			}
			catch(std::exception &e) {
				std::cerr << "Error : " << e.what() << std::endl;
			}
		}
		//stream.stop();
		//wavfile.flush();

		rx.disconnect();

#endif

	}
	catch(std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}
	return 0;
}




