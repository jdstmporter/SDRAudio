/*
 * FIRFilter.hpp
 *
 *  Created on: 13 Nov 2020
 *      Author: julianporter
 */

#ifndef FIRFILTER_HPP_
#define FIRFILTER_HPP_

#include <boost/circular_buffer.hpp>

class FIRFilter {
public:
	FIRFilter();
	virtual ~FIRFilter();
};

#endif /* FIRFILTER_HPP_ */
