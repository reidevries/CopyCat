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
	
	void reduce();

	Fraction() : n(0), d(1) {}

	Fraction(int set_n, int set_d) : n(set_n), d(set_d) {reduce();}
	
	//convert double to nearest fraction, capping at given num decimal_places 
	Fraction(const double target, const int decimal_places);
	
	Fraction(double target) : Fraction(target, 100) {}

	int geti() {return (n)/d;}
	float getf() {return static_cast<float>(n)/static_cast<float>(d);}
	double getd() {return static_cast<double>(n)/static_cast<double>(d);}
	
	Fraction operator+(const Fraction& b);
	Fraction operator+=(const Fraction& b);
	Fraction operator+(int b);
	Fraction operator+=(int b);
	Fraction operator-(const Fraction& b);
	Fraction operator-=(const Fraction& b);
	Fraction operator-(int b);
	Fraction operator-=(int b);
	Fraction operator*(const Fraction& b);
	Fraction operator*=(const Fraction& b);
	Fraction operator*(int b);
	Fraction operator*=(int b);
	Fraction operator/(const Fraction& b);
	Fraction operator/(int b);
};

std::ostream& operator<<(std::ostream& a, const Fraction& b);

#endif
