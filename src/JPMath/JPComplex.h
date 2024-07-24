#ifndef INC_JPCOMPLEX_H
#define INC_JPCOMPLEX_H

#include "stdafx.h"

class JPComplex {
public:
	inline JPComplex(double r, double i);
	inline JPComplex(const JPComplex &c);
	inline JPComplex(double r);
	inline JPComplex();

	inline JPComplex& set(const JPComplex &c);
	inline JPComplex& set(double r);

	inline JPComplex& operator = (const JPComplex &c);
	inline JPComplex& operator = (double r);

	inline bool operator == (const JPComplex &c);
	inline bool operator != (const JPComplex &c);
	inline bool operator == (double r);
	inline bool operator != (double r);

	inline JPComplex& operator += (const JPComplex &c);
	inline JPComplex& operator -= (const JPComplex &c);
	inline JPComplex& operator *= (const JPComplex &c);
	inline JPComplex& operator /= (const JPComplex &c);

	inline JPComplex& operator += (double r);
	inline JPComplex& operator -= (double r);
	inline JPComplex& operator *= (double r);
	inline JPComplex& operator /= (double r);

	inline void writeComplex();

	inline bool isReal();
	inline bool isComplex();
	inline bool isPureImaginary();

	double re;
	double im;
};

inline double abs(const JPComplex &c);
inline JPComplex complexConjugate(const JPComplex &c);
inline JPComplex sqrt(const JPComplex &c);
inline JPComplex sqrtToComplex(const double r);

inline JPComplex operator + (const JPComplex &a, const JPComplex &b);
inline JPComplex operator - (const JPComplex &a, const JPComplex &b);
inline JPComplex operator * (const JPComplex &a, const JPComplex &b);
inline JPComplex operator / (const JPComplex &a, const JPComplex &b);

inline JPComplex operator + (const JPComplex &a, double r);
inline JPComplex operator - (const JPComplex &a, double r);
inline JPComplex operator * (const JPComplex &a, double r);
inline JPComplex operator / (const JPComplex &a, double r);

inline JPComplex operator + (double r, const JPComplex &b);
inline JPComplex operator - (double r, const JPComplex &b);
inline JPComplex operator * (double r, const JPComplex &b);
inline JPComplex operator / (double r, const JPComplex &b);

inline JPComplex::JPComplex(double r, double i){
	re = r;
	im = i;
}

inline JPComplex::JPComplex(const JPComplex &c){
	re = c.re;
	im = c.im;
}

inline JPComplex::JPComplex(double r){
	re = r;
	im = 0.0;
}

inline JPComplex::JPComplex(){
	re = 0.0;
	im = 0.0;
}

inline JPComplex& JPComplex::set(const JPComplex &c){
	re = c.re;
	im = c.im;
	return (*this);
}

inline JPComplex& JPComplex::set(double r){
	re = r;
	im = 0;
	return (*this);
}

inline JPComplex& JPComplex::operator = (const JPComplex &c){
	return set(c);
}

inline JPComplex& JPComplex::operator = (double r){
	return set(r);
}

inline bool JPComplex::operator == (const JPComplex &c){
	return (re == c.re && im == c.im);
}

inline bool JPComplex::operator != (const JPComplex &c){
	return (re != c.re || im != c.im);
}

inline bool JPComplex::operator == (double r){
	return (re == r && im == 0);
}

inline bool JPComplex::operator != (double r){
	return (re != r || im != 0);
}

inline JPComplex& JPComplex::operator += (const JPComplex &c){
	re += c.re;
	im += c.im;
	return (*this);
}

inline JPComplex& JPComplex::operator -= (const JPComplex &c){
	re += c.re;
	im += c.im;
	return (*this);
}

inline JPComplex& JPComplex::operator *= (const JPComplex &c){
	double tempre;
	tempre = re*c.re - im*c.im;
	im	   = re*c.im + im*c.re;
	re = tempre;
	return (*this);
}

inline JPComplex& JPComplex::operator /= (const JPComplex &c){
	double tempre;
	double r,den;
	if (fabs(c.re) >= fabs(c.im)) {
		r= c.im/c.re;
		den = c.re + r*c.im;
		tempre = (re + r*im)/den;
		im     = (im - r*re)/den;
		re = tempre;
	} 
	else {
		r = c.re/c.im;
		den = c.im + r*c.re;
		tempre =(re*r + im)/den;
		im     =(im*r - re)/den;
		re = tempre;
	}
	return (*this);
}

inline JPComplex& JPComplex::operator += (double r){
	re += r;
	return (*this);
}

inline JPComplex& JPComplex::operator -= (double r){
	re -= r;
	return (*this);
}

inline JPComplex& JPComplex::operator *= (double r){
	re *= r;
	im *= r;
	return (*this);
}

inline JPComplex& JPComplex::operator /= (double r){
	re /= r;
	im /= r;
	return (*this);
}

inline bool JPComplex::isReal(){
	return (im == 0.0);
}

inline bool JPComplex::isComplex(){
	return (re != 0.0 && im != 0.0);
}

inline bool JPComplex::isPureImaginary(){
	return (re == 0.0 && im != 0.0);
}


inline JPComplex operator + (const JPComplex &a, const JPComplex &b){
	return JPComplex(a.re+b.re, a.im+b.im);
}

inline JPComplex operator - (const JPComplex &a, const JPComplex &b){
	return JPComplex(a.re-b.re, a.im-b.im);
}

inline JPComplex operator * (const JPComplex &a, const JPComplex &b){
	return JPComplex(a.re*b.re-a.im*b.im, a.re*b.im+a.im*b.re);
}

inline JPComplex operator / (const JPComplex &a, const JPComplex &b){
	JPComplex c;
	double r,den;
	if (fabs(b.re) >= fabs(b.im)) {
		r= b.im/b.re;
		den = b.re + r*b.im;
		c.re = (a.re + r*a.im)/den;
		c.im = (a.im - r*a.re)/den;
	} 
	else {
		r = b.re/b.im;
		den = b.im + r*b.re;
		c.re =(a.re*r + a.im)/den;
		c.im =(a.im*r - a.re)/den;
	}
	return c;
}

inline JPComplex operator + (const JPComplex &a, double r){
	return JPComplex(a.re + r, a.im);
}

inline JPComplex operator - (const JPComplex &a, double r){
	return JPComplex(a.re - r, a.im);
}

inline JPComplex operator * (const JPComplex &a, double r){
	return JPComplex(a.re*r, a.im*r);
}

inline JPComplex operator / (const JPComplex &a, double r){
	return JPComplex(a.re/r, a.im/r);
}

inline JPComplex operator + (double r, const JPComplex &b){
	return JPComplex(b.re + r, b.im);
}

inline JPComplex operator - (double r, const JPComplex &b){
	return JPComplex(r - b.re, -b.im);
}

inline JPComplex operator * (double r, const JPComplex &b){
	return JPComplex(b.re*r, b.im*r);
}

inline JPComplex operator / (double d, const JPComplex &b){
	JPComplex c;
	double r,den;
	if (fabs(b.re) >= fabs(b.im)) {
		r = b.im/b.re;
		den = b.re + r*b.im;
		c.re = (d)/den;
		c.im = (-r*d)/den;
	} 
	else {
		r = b.re/b.im;
		den = b.im + r*b.re;
		c.re = (d*r)/den;
		c.im = (-d)/den;
	}
	return c;
}

inline double abs(const JPComplex &c){
	double x,y,ans,temp;
	x = fabs(c.re);
	y = fabs(c.im);
	if(x == 0.0)
		ans=y;
	else if(y == 0.0)
	ans = x;
	else if(x > y){
		temp = y/x;
		ans=x*sqrt(1.0+temp*temp);
	}
	else{
		temp = x/y;
		ans = y*sqrt(1.0+temp*temp);
	}
	return ans;
}

inline JPComplex complexConjugate(const JPComplex &c){
	return JPComplex(c.re, -c.im);
}

inline JPComplex sqrt(const JPComplex &z){
	JPComplex c;
	double x,y,w,r;
	if ((z.re == 0.0) && (z.im == 0.0)) {
		c.re = 0.0;
		c.im = 0.0;
		return c;
	}
	else{
		x = fabs(z.re);
		y = fabs(z.im);
		if (x >= y) {
			r=y/x;
			w = sqrt(x)*sqrt(0.5*(1.0+sqrt(1.0+r*r)));
		} else {
			r=x/y;
			w = sqrt(y)*sqrt(0.5*(r+sqrt(1.0+r*r)));
		}
		if(z.re >= 0.0){
			c.re = w;
			c.im = z.im/(2.0*w);
		}
		else{
			c.im = (z.im >= 0) ? w : -w;
			c.re = z.im/(2.0*c.im);
		}
		return c;
	}
}

inline JPComplex sqrtToComplex(const double r){
	return sqrt(JPComplex(r));
}

// note: the output parameter "roots" must be of size n
inline void nthRoots(const JPComplex &z, int n, JPComplex roots[]){
	double theta = atan2(z.im, z.re);
	double r = sqrt(z.re*z.re + z.im*z.im);
	double one_over_n = 1.0 / double(n);
	double theta_over_n = one_over_n*theta;
	double pow_r_one_over_n = pow(r, one_over_n);
	
	for(int k = 0; k < n; k++){
		double alpha = theta_over_n + 2*k*one_over_n;
		roots[k] = pow_r_one_over_n*JPComplex(cos(alpha), sin(alpha));
	}
}

inline JPComplex nthRoot_k(const JPComplex &z, int n, int k){
	double theta = atan2(z.im, z.re);
	double r = sqrt(z.re*z.re + z.im*z.im);
	double one_over_n = 1.0 / double(n);
	double theta_over_n = one_over_n*(theta);
	double pow_r_one_over_n = pow(r, one_over_n);
	
	double alpha = theta_over_n + 2*k*one_over_n;
	return pow_r_one_over_n*JPComplex(cos(alpha), sin(alpha));
}




#endif

