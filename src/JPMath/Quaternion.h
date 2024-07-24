#ifndef INC_QUATERION_H
#define INC_QUATERION_H

#include <iomanip>
#include "Vector3.h"
#include "Matrix33.h"
#include "JPMathConstants.h"

#include <string>
#include <sstream>
#include <istream>
#include <iostream>

class Quaternion
{
public:

	inline Quaternion(void) : x(0),y(0),z(0),w(1) {}
	inline Quaternion(const double newX,const double newY,const double newZ,const double newW);
	inline Quaternion(Vec3 axis,double angle);
	inline Quaternion(Vec3 &X, Vec3 &Y, Vec3 &Z);
	inline Quaternion(double yaw, double pitch, double roll);
	inline Quaternion(const Matrix33 &m);

	inline Quaternion &operator () (const double newX,const double newY,const double newZ);
	inline Quaternion &operator () (const double newX,const double newY,const double newZ,const double newW);
	inline Quaternion &operator () (Quaternion &other);
	inline Quaternion &operator () (Vec3 axis,double angle);
	inline Quaternion &operator = (const Quaternion &other);
	inline Quaternion &operator = (const Matrix33 &m);
	inline Quaternion &operator ~ ();
	inline Quaternion &set(double newX,double newY,double newZ,double newW);

	inline bool operator == (Quaternion &other);
	inline bool operator != (Quaternion &other);

	inline Quaternion operator - ();
	inline Quaternion operator + (Quaternion &other);
	inline Quaternion operator - (Quaternion &other);
	inline Quaternion operator * (Quaternion &other);

	inline Quaternion &operator += (Quaternion &other);
	inline Quaternion &operator -= (Quaternion &other);
	inline Quaternion &operator *= (Quaternion &other);

	inline Quaternion &operator /= (double &scalar);
	inline Quaternion &operator *= (double &scalar);

	inline Quaternion &setEuler(double yaw, double pitch, double roll);
	inline Matrix33 getMatrix(void);
	inline void getFrame(Vec3 &LX, Vec3 &LY, Vec3 &LZ);
	inline void getAxisAngle(Vec3 &axis, double *angle);
	inline Quaternion getShortestArcQuat();
	inline double rotationAngle();
	inline Vec3 applyTo(Vec3 &v);


	union{
		double elements[4];
		struct{
			double w,x,y,z;
		};
	};

};

inline Quaternion compose (const Quaternion &first, const Quaternion &second);
inline Quaternion operator * (const Quaternion &first, const Quaternion &second);
inline Quaternion operator + (const Quaternion &first, const Quaternion &second);
inline Quaternion operator - (const Quaternion &first, const Quaternion &second);
inline Quaternion operator - (const Quaternion &quat);

inline Quaternion operator * (const Quaternion &quat,double scalar);
inline Quaternion operator * (double scalar,const Quaternion &quat);

inline Quaternion rotationArc(Vec3 V0, Vec3 V1);
inline double dot(const Quaternion &first, const Quaternion &second);
inline Quaternion slerp(const Quaternion &from, const Quaternion &to, double Interpolation);

inline void normalize(Quaternion &quat);
inline double length(const Quaternion &quat);
inline Quaternion inverse(Quaternion &quat);

inline Quaternion::Quaternion(const double newX,const double newY,const double newZ,const double newW){
	set(newX,newY,newZ,newW);
}

inline Quaternion::Quaternion(Vec3 axis,double angle){
	double temp1,temp2;
	temp1 = length(axis);
	if(temp1 == 0){
		printf("Quaternion axis is zero!");
	}
	temp2 = sin(angle * 0.5) / temp1;
	set(axis.x*temp2, axis.y*temp2, axis.z*temp2, cos(angle*0.5));
}

inline Quaternion::Quaternion(Vec3 &X, Vec3 &Y, Vec3 &Z){
	double &m00(X.x), &m10(X.y), &m20(X.z), &m01(Y.x), &m11(Y.y), &m21(Y.z), &m02(Z.x), &m12(Z.y), &m22(Z.z);
	
	w = 1.0 + m00 + m11 + m22;
	if(w < 0.0) w = 0.0;
	w = sqrt(w) / 2.0;
	double w4 = (4.0 * w);
	x = (m21 - m12) / w4 ;
	y = (m02 - m20) / w4 ;
	z = (m10 - m01) / w4 ;
}

inline Quaternion &Quaternion::operator () (const double newX,const double newY,const double newZ){
	set(newX,newY,newZ,w);
	return *this;
}

inline Quaternion &Quaternion::operator () (const double newX,const double newY,const double newZ,const double newW){
	set(newX,newY,newZ,newW);
	return *this;
}

inline Quaternion &Quaternion::operator () (Quaternion &other){
	set(other.x,other.y,other.z,other.w);
	return *this;
}

inline Quaternion &Quaternion::operator () (Vec3 axis,double angle){
	double temp1,temp2;
	temp1 = length(axis);
	if(temp1 == 0){
		printf("Quaternion axis is zero!");
	}
	temp2 = sin(angle * 0.5F) / temp1;
	set(axis.x * temp2,axis.y * temp2,axis.z * temp2,cos(angle * 0.5F));
	return *this;
}

inline Quaternion::Quaternion(const Matrix33 &m){
	const double trace = 1.0f + m.elements[0][0] + m.elements[1][1] + m.elements[2][2];

	if (trace > 0.00001f)
	{
		const double s = sqrt(trace) * 2;
		x = (m.elements[2][1] - m.elements[1][2]) / s,
		y = (m.elements[0][2] - m.elements[2][0]) / s,
		z = (m.elements[1][0] - m.elements[0][1]) / s,
		w = s / 4;
	}
	else if (m.elements[0][0] > m.elements[1][1] && m.elements[0][0] > m.elements[2][2])
	{
		const double s = sqrt(1.0f + m.elements[0][0] - m.elements[1][1] - m.elements[2][2]) * 2;
		x = s / 4,
		y = (m.elements[1][0] + m.elements[0][1]) / s,
		z = (m.elements[0][2] + m.elements[2][0]) / s,
		w = (m.elements[2][1] - m.elements[1][2]) / s;
	}
	else if (m.elements[1][1] > m.elements[2][2])
	{
		const double s = sqrt(1.0f + m.elements[1][1] - m.elements[0][0] - m.elements[2][2]) * 2;
		x = (m.elements[1][0] + m.elements[0][1]) / s,
		y = s / 4,
		z = (m.elements[2][1] + m.elements[1][2]) / s,
		w = (m.elements[0][2] - m.elements[2][0]) / s;
	}
	else
	{
		const double s = sqrt(1.0f + m.elements[2][2] - m.elements[0][0] - m.elements[1][1]) * 2;
		x = (m.elements[0][2] + m.elements[2][0]) / s,
		y = (m.elements[2][1] + m.elements[1][2]) / s,
		z = s / 4,
		w = (m.elements[1][0] - m.elements[0][1]) / s;
	}
	
}

inline Quaternion &Quaternion::operator = (const Quaternion &other){
	set(other.x,other.y,other.z,other.w);
	return *this;
}

inline Quaternion &Quaternion::operator = (const Matrix33 &m){
	w = 1.0 + m.elements[0][0] + m.elements[1][1] + m.elements[2][2];
	if(w < 0.0) w = 0.0;
	w = sqrt(w) / 2.0;
	double w4 = (4.0 * w);
	x = (m.elements[2][1] - m.elements[1][2]) / w4 ;
	y = (m.elements[0][2] - m.elements[2][0]) / w4 ;
	z = (m.elements[1][0] - m.elements[0][1]) / w4 ;
	return *this;
}

inline Quaternion &Quaternion::operator ~ (){
	x=-x;
	y=-y;
	z=-z;
	return *this;
}

inline Quaternion &Quaternion::set(double newX,double newY,double newZ,double newW){
	x=newX;
	y=newY;
	z=newZ;
	w=newW;
	return *this;
}

inline bool Quaternion::operator == (Quaternion &other){
	if ((x==other.x) && (y==other.y) && (z==other.z) && (w==other.w))
		return true;
	return false;
}

inline bool Quaternion::operator != (Quaternion &other){
	if ((x!=other.x) || (y!=other.y) || (z!=other.z) || (w!=other.w))
		return true;
	return false;
}

inline Quaternion Quaternion::operator - (){
	return Quaternion(-x,-y,-z,-w);
}

inline Quaternion Quaternion::operator + (Quaternion &other){
	Quaternion temp;
	temp(other.x+x,other.y+y,other.z+z,other.w+w);
	return temp;
}

inline Quaternion Quaternion::operator - (Quaternion &other){
	Quaternion temp;
	temp(other.x-x,other.y-y,other.z-z,other.w-w);
	return temp;
}

inline Quaternion Quaternion::operator * (Quaternion &other){
	return Quaternion(
		w * other.x + x * other.w + y * other.z - z * other.y,
		w * other.y + y * other.w + z * other.x - x * other.z,
		w * other.z + z * other.w + x * other.y - y * other.x,
		w * other.w - x * other.x - y * other.y - z * other.z); 
}

inline Quaternion &Quaternion::operator += (Quaternion &other){
	x+=other.x;
	y+=other.y;
	z+=other.z;
	w+=other.w;
	return *this;
}

inline Quaternion &Quaternion::operator -= (Quaternion &other){
	x-=other.x;
	y-=other.y;
	z-=other.z;
	w-=other.w;
	return *this;
}

inline Quaternion &Quaternion::operator *= (Quaternion &other){
	x=w * other.x + x * other.w + y * other.z - z * other.y,
	y=w * other.y + y * other.w + z * other.x - x * other.z,
	z=w * other.z + z * other.w + x * other.y - y * other.x,
	w=w * other.w - x * other.x - y * other.y - z * other.z; 
	return *this;
}

inline Quaternion &Quaternion::operator /= (double &Scalar){
	return set(x/Scalar,y/Scalar,z/Scalar,w/Scalar);
}

inline Quaternion &Quaternion::operator *= (double &Scalar){
	return set(x*Scalar,y*Scalar,z*Scalar,w*Scalar);
}

inline Quaternion &Quaternion::setEuler(double yaw, double Pitch, double Roll){
	double cosy = cos(yaw / 2.0F);
	double siny = sin(yaw / 2.0F);
	double cosP = cos(Pitch / 2.0F);
	double sinP = sin(Pitch / 2.0F);
	double cosR = cos(Roll / 2.0F);
	double sinR = sin(Roll / 2.0F);
	set(
		cosR * sinP * cosy + sinR * cosP * siny,
		cosR * cosP * siny - sinR * sinP * cosy,
		sinR * cosP * cosy - cosR * sinP * siny,
		cosR * cosP * cosy + sinR * sinP * siny);
	return *this;
}

inline Matrix33 Quaternion::getMatrix(void){
	double wx = w*x;
	double wy = w*y;
	double wz = w*z;
	double xx = x*x;
	double xy = x*y;
	double xz = x*z;
	double yy = y*y;
	double yz = y*z;
	double zz = z*z;
	return Matrix33(1-2*(yy+zz),	2*(xy-wz),		2*(xz+wy),
					2*(xy+wz),		1-2*(xx+zz),	2*(yz-wx),
					2*(xz-wy),		2*(yz+wx),		1-2*(xx+yy));
}

inline void Quaternion::getFrame(Vec3 &LX, Vec3 &LY, Vec3 &LZ){
	double wx = w*x;
	double wy = w*y;
	double wz = w*z;
	double xx = x*x;
	double xy = x*y;
	double xz = x*z;
	double yy = y*y;
	double yz = y*z;
	double zz = z*z;
	LX.set(1-2*(yy+zz), 2*(xy+wz),   2*(xz-wy));
	LY.set(2*(xy-wz),   1-2*(xx+zz), 2*(yz+wx));
	LZ.set(2*(xz+wy),   2*(yz-wx),   1-2*(xx+yy));
}

inline void Quaternion::getAxisAngle(Vec3 &axis, double *angle){
	Quaternion q(*this);
	if (q.w > 1)
		normalize(q);
	*angle = 2 * acos(q.w);
	double s = sqrt(1-q.w*q.w);
	if (s < 0.001) { 
		axis.x = q.x; 
		axis.y = q.y;
		axis.z = q.z;
	}
	else {
		axis.x = q.x / s;
		axis.y = q.y / s;
		axis.z = q.z / s;
	}
}



inline double Quaternion::rotationAngle(){
	Quaternion q(*this);
	if (q.w > 1)
		normalize(q);
	return 2 * acos(q.w);
}

inline Quaternion Quaternion::getShortestArcQuat(){
	double rotAngle;
	Vec3 axis;
	getAxisAngle(axis, &rotAngle);
	if(rotAngle > M_PI){
		rotAngle -= 2*M_PI;
		return Quaternion(axis, rotAngle);
	}
	else{
		return *this;
	}
	
}


inline Quaternion operator * (const Quaternion &first, const Quaternion &second){
	return Quaternion(
			first.w * second.x + first.x * second.w + first.y * second.z - first.z * second.y,
			first.w * second.y + first.y * second.w + first.z * second.x - first.x * second.z,
			first.w * second.z + first.z * second.w + first.x * second.y - first.y * second.x,
			first.w * second.w - first.x * second.x - first.y * second.y - first.z * second.z); 
}

inline Quaternion compose(const Quaternion &first, const Quaternion &second){
	Quaternion result;

	Vec3 v1(first.x, first.y, first.z);
	Vec3 v2(second.x, second.y, second.z);

	Vec3 v3 = v1*second.w + v2*first.w + cross(v2, v1);

	result.x = v3.x;
	result.y = v3.y;
	result.z = v3.z;
	result.w = first.w*second.w - (first.x*second.x + first.y*second.y + first.z*second.z);

	return result;
}

inline Quaternion operator + (const Quaternion &first, const Quaternion &second){
	return Quaternion(first.x+second.x,first.y+second.y,first.z+second.z,first.w+second.w);
}

inline Quaternion operator - (const Quaternion &first, const Quaternion &second){
	return Quaternion(first.x-second.x,first.y-second.y,first.z-second.z,first.w-second.w);
}

inline Quaternion operator - (const Quaternion &quat){
	return Quaternion(-quat.x, -quat.y, -quat.z, -quat.w);
}

inline Quaternion operator * (const Quaternion &quat,double Scalar){
	return Quaternion(quat.x*Scalar,quat.y*Scalar,quat.z*Scalar,quat.w*Scalar);
}

inline Quaternion operator * (double Scalar,const Quaternion &quat){
	return Quaternion(quat.x*Scalar,quat.y*Scalar,quat.z*Scalar,quat.w*Scalar);
}

inline Quaternion rotationArc(Vec3 V0,Vec3 V1){
	Quaternion quat;

	Vec3 temp = cross(V0,V1);

	double   d = dot(V0,V1);
	double   s = (double)sqrt((1+d)*2);
	quat.x = temp.x / s;
	quat.y = temp.y / s;
	quat.z = temp.z / s;
	quat.w = s /2.0f;

	return quat;
}

double dot(const Quaternion &first, const Quaternion &second){
	return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
}

inline Quaternion slerp(const Quaternion &from, const Quaternion &to, double Interpolation){
	Quaternion temp;
	double omega, cosO, sinO;
	double scale0, scale1;

	cosO = dot(from, to);

	if (cosO < 0.0){
		cosO = -cosO;
		temp = -to;
	}
	else{
		temp = to;
	}

	if ((1.0 - cosO) > EPS){
		omega = (double)acos(cosO);
		sinO = sin(omega);
		scale0 = sin((1.0F - Interpolation) * omega) / sinO;
		scale1 = sin(Interpolation * omega) / sinO;
	}
	else{
		scale0 = 1.0F - Interpolation;
		scale1 = Interpolation;
	}
	return from*scale0 + temp*scale1;
}

inline void normalize(Quaternion &quat){
	double l = length(quat);
	if(l < EPS){
		quat.x = 0;
		quat.y = 0;
		quat.z = 0;
		quat.w = 0;
		return;
	}
	quat.x /= l;
	quat.y /= l;
	quat.z /= l;
	quat.w /= l;
}

inline double length(const Quaternion &quat){
	return sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
}


inline Quaternion inverse(Quaternion &quat){
	double mag2(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
	return Quaternion(-quat.x/mag2, -quat.y/mag2, -quat.z/mag2, quat.w/mag2);
}

inline ostream& operator<< (ostream &ostr, Quaternion &q){
	ostr << "<" << q.w << " " << q.x << " " << q.y << " " << q.z << ">";
	return ostr;
}

inline istream& operator>> (istream &istr, Quaternion &q){
	char pat[1024];
	istr >> setw(strlen("<")+1) >> pat >> q.w >> q.x >> q.y >> q.z >> setw(strlen("<")+1) >> pat;
	return istr;
}

inline Vec3 Quaternion::applyTo(Vec3 &v){
	double wx = w*x;
	double wy = w*y;
	double wz = w*z;
	double xx = x*x;
	double xy = x*y;
	double xz = x*z;
	double yy = y*y;
	double yz = y*z;
	double zz = z*z;
	return Vec3(1-2*(yy+zz)*v.x + 2*(xy-wz)*v.y + 2*(xz+wy)*v.z,
			2*(xy+wz)*v.x + 1-2*(xx+zz)*v.y + 2*(yz-wx)*v.z,
			2*(xz-wy)*v.x + 2*(yz+wx)*v.y + 1-2*(xx+yy)*v.z);
}

#endif