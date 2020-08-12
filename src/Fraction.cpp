/*
 * Fraction.cpp
 */
 
#include "Fraction.hpp"

using namespace std;

Fraction::Fraction(const double target, const int decimal_places)
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

void Fraction::reduce()
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

Fraction Fraction::operator+(const Fraction& b) 
{
	return Fraction(n*b.d+b.n*d, b.d*d);
}

Fraction Fraction::operator+=(const Fraction& b) 
{
	n = n*b.d+b.n*d;
	d = b.d*d;
	reduce();
	return *this;
}

Fraction Fraction::operator+(int b)
{
	return Fraction(n+d*b,d);
}

Fraction Fraction::operator+=(int b)
{
	n = n+d*b;
	reduce();
	return *this;
}

Fraction Fraction::operator-(const Fraction& b) 
{
	return Fraction(n*b.d-b.n*d, b.d*d);
}

Fraction Fraction::operator-=(const Fraction& b) 
{
	n = n*b.d-b.n*d;
	d = b.d*d;
	reduce();
	return *this;
}

Fraction Fraction::operator-(int b)
{
	return Fraction(n-d*b,d);
}

Fraction Fraction::operator-=(int b)
{
	n = n-d*b;
	reduce();
	return *this;
}

Fraction Fraction::operator*(const Fraction& b) 
{
	return Fraction(n*b.n,d*b.d);
}

Fraction Fraction::operator*=(const Fraction& b) 
{
	n *= b.n;
	d *= b.d;
	reduce();
	return *this;
}

Fraction Fraction::operator*(int b)
{
	if (b == 0) {
		return Fraction(0,1);
	} else {
		return Fraction(n*b,d); //no big deal
	}
}

Fraction Fraction::operator*=(int b)
{
	n*=b;
	reduce();
	return *this;
}

Fraction Fraction::operator/(const Fraction& b)
{
	return Fraction(n*b.d,d*b.n);
}

Fraction Fraction::operator/(int b)
{
	return Fraction(n/b,d);
}

std::ostream& operator<<(std::ostream& a, const Fraction& b)
{
	if (b.d == 1) {
		a << b.n;
	} else {
		a << b.n << "/" << b.d;
	}
	return a;
}
