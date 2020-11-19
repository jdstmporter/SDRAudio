/*
 * SDRDecimator.hpp
 *
 *  Created on: 4 Nov 2020
 *      Author: julianporter
 */

#ifndef SDRDECIMATOR_HPP_
#define SDRDECIMATOR_HPP_

#include "base.hpp"
#include <memory>
#include <type_traits>

namespace sdr {

class SDRDecimatorBase{
public:
protected:
	unsigned long factor;
	float inF, outF;
	unsigned inBlock;
	unsigned outBlock;

	CXData inputs;
	CXData outputs;

public:
	SDRDecimatorBase(const float inFrequency,
					 const float outFrequency,
					 const unsigned long factor_,
					 const unsigned long blockSize) :
			factor(factor_),
			inF(inFrequency), outF(inFrequency),
			inBlock(blockSize),
			outBlock(std::ceil(blockSize/(float)factor)),
			inputs(inBlock),
			outputs(outBlock) {};
	SDRDecimatorBase(const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_) :
		SDRDecimatorBase(inFrequency,inFrequency/(float)factor_,factor_,blockSize) {};
	SDRDecimatorBase(const float inFrequency,
					 const float outFrequency,
					 const unsigned long blockSize) :
		SDRDecimatorBase(inFrequency,outFrequency,std::ceil(inFrequency/outFrequency),blockSize) {};

	SDRDecimatorBase & operator=(const SDRDecimatorBase &other) = default;
	SDRDecimatorBase(const SDRDecimatorBase &other) = default;
	virtual ~SDRDecimatorBase() = default;



	virtual void operator()(const cx_t *in) = 0;
	virtual void operator()(const std::vector<cx_t> &in) {
		(*this)(in.data());
	}

	float operator[](const size_t idx) const { return outputs[idx]; }
	unsigned outSize() const { return outputs.size(); }
	unsigned inSize() const { return inBlock; }

	void copy(float *v) const {
		for(auto i=0;i<2*outputs.size();i++) v[i] = outputs[i];
	}

	void copy(std::vector<float> &v) const {
		copy(v.data());
	}

};

class SDRSimpleDecimator : public SDRDecimatorBase {

public:
	SDRSimpleDecimator(const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_) :
		SDRDecimatorBase(inFrequency,blockSize,factor_) {};
	SDRSimpleDecimator(const float inFrequency,
				 const float outFrequency,
				 const unsigned long blockSize) :
		SDRDecimatorBase(inFrequency,outFrequency,blockSize) {};
	virtual ~SDRSimpleDecimator() = default;

	virtual void operator()(const cx_t *in) {};
};

class SDRSimpleLPFDecimator : public SDRDecimatorBase {

public:
	SDRSimpleLPFDecimator(const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_) :
		SDRDecimatorBase(inFrequency,blockSize,factor_) {};
	SDRSimpleLPFDecimator(const float inFrequency,
				 const float outFrequency,
				 const unsigned long blockSize) :
		SDRDecimatorBase(inFrequency,outFrequency,blockSize) {};
	virtual ~SDRSimpleLPFDecimator() = default;

	virtual void operator()(const cx_t *in) {};
};

class SDRDecimator : public SDRDecimatorBase {
private:
	using freq_t = kfr::size_t;
	kfr::samplerate_converter<float> decimatorR, decimatorI;

public:
	SDRDecimator(const float inFrequency,
				 const unsigned long blockSize,
				 const unsigned long factor_,
				 const kfr::resample_quality quality=kfr::resample_quality::high);
	SDRDecimator(const float inFrequency,
				 const float outFrequency,
				 const unsigned long blockSize,
				 const kfr::resample_quality quality=kfr::resample_quality::high);
	virtual ~SDRDecimator() = default;

	virtual void operator()(const cx_t *in) = 0;
};

template<typename T,
		 class = typename std::enable_if_t<std::is_base_of_v<SDRDecimatorBase,T>>,
		 typename... Args>
std::shared_ptr<SDRDecimatorBase> Decimator(Args &&... args) {
	return std::static_pointer_cast<SDRDecimatorBase>(std::make_shared<T>(args...));
}

} /* namespace sdr */

#endif /* SDRDECIMATOR_HPP_ */
