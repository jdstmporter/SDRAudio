/*
 * main.cpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#include "SDRReceiver.hpp"

int main(const int argc,const char **argv) {
	try {
		auto centreFrequency = std::stof(argv[1])*1.0e6;

		sdr::SDRReceiver rx(0,centreFrequency,10.0);
		rx.connect();

		for(auto i=0;i<10;i++) {
			try {
				auto response = rx.load();
				std::cout << i << ": " << response << std::endl;
			}
			catch(sdr::SDRError &e) {
				std::cerr << i << ": " << e.what() << std::endl;
			}
			catch(std::exception &e) {
				std::cerr << i << ": " << e.what() << std::endl;
			}
		}
		rx.disconnect();
	}
	catch(std::exception &e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}

}




