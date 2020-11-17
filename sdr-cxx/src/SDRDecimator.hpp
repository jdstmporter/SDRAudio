/*
 * SDRDecimator.hpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRDECIMATOR_HPP_
#define SDRDECIMATOR_HPP_

#include "base.hpp"

namespace sdr {

class SDRDecimator {
public:
private:
	float factor;
	unsigned long long outF, inF;
		kfr::samplerate_converter<float> decimatorR, decimatorI;
		unsigned inBlock;
		unsigned outBlock;

	CXData inputs;
	CXData outputs;

public:
	SDRDecimator(const unsigned long long outFrequency,
				 const unsigned long long inFrequency,
				 const unsigned blockSize=1024,
				 const kfr::resample_quality quality=kfr::resample_quality::high);
	SDRDecimator() : SDRDecimator(48000,96000) {};
	virtual ~SDRDecimator();
	SDRDecimator(SDRDecimator &&other) = default;
	SDRDecimator(const SDRDecimator &other) = default;


	void operator()(const cx_t *in);
	void operator()(const std::vector<cx_t> &in);

	float operator[](const unsigned long long idx) const { return outputs[idx]; }
	unsigned outSize() const { return outputs.size(); }
	unsigned inSize() const { return inBlock; }

	void copy(std::vector<float>::iterator it) {
		for(auto i=0;i<2*outputs.size();i++) {
			*it++ = outputs[i];
		}
	}
};

} /* namespace sdr */

#endif /* SDRDECIMATOR_HPP_ */
