/*
 * base.hpp
 *
 *  Created on: 2 Nov 2020
 *      Author: julianporter
 */

#ifndef BASE_HPP_
#define BASE_HPP_

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <complex>
#include <vector>
#include <algorithm>
#include <iterator>

#include <kfr/base.hpp>
#include <kfr/dsp.hpp>

namespace sdr {
using cx_t = std::complex<float>;
using vec_t = kfr::univector<float>;

constexpr cx_t cx_zero = cx_t(0,0);
constexpr cx_t cx_one = cx_t(1,0);
constexpr cx_t cx_i = cx_t(0,1);

constexpr float PI = 3.14159256;

cx_t r2cx(const float) noexcept;

struct CXData {
	using size_t = unsigned long long;

	class iterator : public std::iterator<std::forward_iterator_tag,float,long long> {
	private:
		long long offset;
		long long size;
		vec_t::iterator &reals;
		vec_t::iterator &imags;

		float at() const;
	public:
		iterator(CXData &d,long long o=0) :
			offset(o), size(2*d.size()), reals(d.beginR()), imags(d.beginI()) {};
		iterator(const iterator &o) :
			offset(o.offset), size(o.size), reals(o.reals), imags(o.imags) {};
		iterator & operator=(const iterator &o);
		virtual ~iterator() = default;

		iterator & operator++();
		iterator & operator++(int);
		float operator*() const { return at(); }
		float operator->() const { return at(); }
		bool operator==(const iterator &o) const {
			return offset==o.offset && size==o.size;
		}
		bool operator!=(const iterator &o) const {
			return offset != o.offset || size != o.size;
		}
	};

	size_t n;
	vec_t real;
	vec_t imag;

	CXData(const std::vector<cx_t> &inputs);
	CXData(const cx_t *inputs,const size_t n_);
	CXData(const size_t n_) : n(n_), real(n,0), imag(n,0) {};
	CXData(const CXData &) = default;
	CXData & operator=(const CXData &) = default;
	virtual ~CXData() = default;

	void load(const cx_t *inputs);
	void load(const std::vector<cx_t> &inputs);

	size_t size() const { return n; }
	vec_t::iterator beginR() { return real.begin(); }
	vec_t::iterator beginI() { return imag.begin(); }
	vec_t::iterator endR() { return real.end(); }
	vec_t::iterator endI() { return imag.end(); }

	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(*this,n); }

	float operator[](const size_t idx) const;

};




}




#endif /* BASE_HPP_ */
