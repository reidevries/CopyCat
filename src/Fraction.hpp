/*
 * Fraction.hpp
 *
 *  Created on: Aug 07, 2020
 *      Author: rei de vries
 *		A struct representing a fraction
 *		based on this https://martin-thoma.com/fractions-in-cpp/
 */

#ifndef FRACTION_HPP_
#define FRACTION_HPP_

#include <exception>
#include <iostream>
#include <cmath>

class Fraction {
private:
	int gcd(int a, int b) const 
	{
		while (a != b) {
			if (a > b) {
				a -= b;
			} else {
				b -= a;
			}
		}
		return a;
	}

public:
	int n, d;
	
	void reduce()
	{
		if (d == 0) {
			throw std::invalid_argument("denominator cannot be zero");
		}
		
		if (n == 0) {
			n = 0;
			d = 1;
		} else {
			int sign = 1;
			if (n < 0) {
				sign *= -1;
				n *= -1;
			}
			if (d < 0) {
				sign *= -1;
				d *= -1;
			}

			int tmp = gcd(n, d);
			n = n/tmp*sign;
			d = d/tmp;
		}
	}

	Fraction() 
	{
		n = 0;
		d = 1;
	}

	Fraction(int set_n, int set_d) : n(set_n), d(set_d)
	{
		reduce();
	}
	
	//convert double to nearest fraction, capping at given num decimal_places 
	Fraction(double target, const int decimal_places)
	{
		double min_dist = 1.0/std::pow(10,static_cast<double>(decimal_places));
		n = 1;
		d = 1;
		double f = n/static_cast<double>(d);
		while ( std::abs(f - target) > min_dist ) {
			if (f < target) {
				++n;
			} else {
				++d;
				if (n==d) ++n;
			}
			
			f = n/static_cast<double>(d);
		}
		reduce();
	}
	
	Fraction(double target) : Fraction(target, 100) {}

	operator int() {return (n)/d;}
	operator float() {return static_cast<float>(n)/static_cast<float>(d);}
	operator double() {return static_cast<double>(n)/static_cast<double>(d);}
	
	Fraction operator+(const Fraction& b) 
	{
		return Fraction(n*b.d+b.n*d, b.d*d);
	}
	
	Fraction operator+=(const Fraction& b) 
	{
		n = n*b.d+b.n*d;
		d = b.d*d;
		reduce();
		return *this;
	}
	
	Fraction operator+(int b)
	{
		return Fraction(n+d*b,d);
	}
	
	Fraction operator+=(int b)
	{
		n = n+d*b;
		reduce();
		return *this;
	}
	
	Fraction operator-(const Fraction& b) 
	{
		return Fraction(n*b.d-b.n*d, b.d*d);
	}
	
	Fraction operator-=(const Fraction& b) 
	{
		n = n*b.d-b.n*d;
		d = b.d*d;
		reduce();
		return *this;
	}
	
	Fraction operator-(int b)
	{
		return Fraction(n-d*b,d);
	}
	
	Fraction operator-=(int b)
	{
		n = n-d*b;
		reduce();
		return *this;
	}
	
	Fraction operator*(const Fraction& b) 
	{
		return Fraction(n*b.n,d*b.n);
	}
	
	Fraction operator*=(const Fraction& b) 
	{
		n *= b.n;
		d *= b.d;
		reduce();
		return *this;
	}
	
	Fraction operator*(int b)
	{
		if (b == 0) {
			return Fraction(0,1);
		} else {
			return Fraction(n*b,d); //no big deal
		}
	}
	
	Fraction operator*=(int b)
	{
		n*=b;
		reduce();
		return *this;
	}
	
	Fraction operator/(const Fraction& b)
	{
		return Fraction(n*b.d,d*b.n);
	}
	
	Fraction operator/(int b)
	{
		return Fraction(n/b,d);
	}
	
	std::ostream& operator<<(std::ostream& strm) 
	{
		if (d == 1) {
			strm << n;
		} else {
			strm << n << "/" << d;
		}
		return strm;
	}
};

#endif
