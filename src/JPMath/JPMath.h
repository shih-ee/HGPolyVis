#ifndef INC_JPMATH_H
#define INC_JPMATH_H
#include "Vector2.h"
#include "Vector3.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix22.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include "Quaternion.h"

typedef Matrix22 Mat2;
typedef Matrix33 Mat3;
typedef Matrix44 Mat4;

bool equals(double val1, double val2, double errTol);
bool lessOrEquals(double val1, double val2, double errTol);
bool greaterOrEquals(double val1, double val2, double errTol);
double normalizeAngle(double angle);
double subtractAngles(double angle1, double angle2);
double acot(double x);
bool lineSegIntersect2D(Vec2 p1, Vec2 p2, Vec2 q1, Vec2 q2, Vec2 *pi, double *sout = NULL, double *tout = NULL);
bool lineSegPlaneIntersect3D(Vec3 p0, Vec3 p1, Vec3 p, Vec3 n, double *sout);
bool rayLineSegIntersect2D(Vec2 rO, Vec2 rD, Vec2 q0, Vec2 q1, Vec2 *ip, double *sout, double *tout);
double degrees(double radians);
double angleVectors2(Vec2 v0, Vec2 v1);
double angleVectors3(Vec3 v03, Vec3 v13);
double angle(Vec2);
double pointLineDist2D(const Vec2 &a, const Vec2 &b, const Vec2 &p, Vec2 &closest);
double pointLineDist3D(const Vec3 &a, const Vec3 &b, const Vec3 &p, Vec3 &closest);
double pointTriangleDistSquared3D(const Vec3 &p, const Vec3 &triV0, const Vec3 &triV1, const Vec3 &triV2, Vec3 &closest);
double pointTriangleDist3D(const Vec3 &p, const Vec3 &triV0, const Vec3 &triV1, const Vec3 &triV2, Vec3 &closest);
bool circleCircleInteresect(Vec2 c0, Vec2 c1, double r0, double r1, Vec2 *pi0, Vec2 *pi1);

template<class T>
inline T linComb(const Vec2 &w, const T &va, const T &vb){
	return w.a*va + w.b*vb;
}

template<class T>
inline T linCombHomogenous(const Vec2 &w, const T &va, const T &vb, const T &vc){
	return w.a*va + w.b*vb + (1.0 - w.a - w.b)*vc;
}

template<class T>
inline T linComb(const Vec3 &w, const T &va, const T &vb, const T &vc){
	return w.a*va + w.b*vb + w.c*vc;
}

template<class T>
inline T linCombHomogenous(const Vec3 &w, const T &va, const T &vb, const T &vc, const T &vd){
	return w.a*va + w.b*vb + w.c*vc + (1.0 - w.a - w.b - w.c)*vd;
}

template<class T>
inline T linComb(const Vec4 &w, const T &va, const T &vb, const T &vc, const T &vd){
	return w.a*va + w.b*vb + w.c*vc + w.d*vd;
}


template<class T>
inline T linComb(double w[], const T &va, const T &vb){
	return w[0]*va + w[1]*vb;
}

template<class T>
inline T linCombHomogenous(double w[], const T &va, const T &vb, const T &vc){
	return w[0]*va + w[1]*vb + (1.0 - w[0] - w[1])*vc;
}

template<class T>
inline T linComb(double w[], const T &va, const T &vb, const T &vc){
	return w[0]*va + w[1]*vb + w[2]*vc;
}

template<class T>
inline T linCombHomogenous(double w[], const T &va, const T &vb, const T &vc, const T &vd){
	return w[0]*va + w[1]*vb + w[2]*vc + (1.0 - w[0] - w[1] - w[2])*vd;
}


template<class T>
inline T linComb(double w[], const T &va, const T &vb, const T &vc, const T &vd){
	return w[0]*va + w[1]*vb + w[2]*vc + w[3]*vd;
}

inline Mat3 pointToTetBarycetricTransMat(const Vec3 &tp0, const Vec3 &tp1, const Vec3 &tp2, const Vec3 &tp3){
	Mat3 TT(tp0.x-tp3.x, tp1.x-tp3.x, tp2.x-tp3.x,
		   tp0.y-tp3.y, tp1.y-tp3.y, tp2.y-tp3.y,
		   tp0.z-tp3.z, tp1.z-tp3.z, tp2.z-tp3.z);

	return inverse(TT);
}

inline Vec3 pointToTetBarycetric(const Vec3 &p, const Vec3 &tp0, const Vec3 &tp1, const Vec3 &tp2, const Vec3 &tp3){
	Mat3 TTinv(pointToTetBarycetricTransMat(tp0, tp1, tp2, tp3));
	return TTinv*p;
}

inline Vec2 pointChangeIntoCoordSys2(const Vec2 &p, const Vec2 &O, const Vec2 &X, const Vec2 &Y){
	Vec2 p_O(p.x-O.x, p.y-O.y);
	return Vec2(dot(p_O, X), dot(p_O, Y));
}

inline Vec2 vectorChangeIntoCoordSys2(const Vec2 &v, const Vec2 &X, const Vec2 &Y){
	return Vec2(dot(v, X), dot(v, Y));
}

inline Vec2 pointChangeOutOfCoordSys2(const Vec2 &p, const Vec2 &O, const Vec2 &X, const Vec2 &Y){
	return O + p.x*X + p.y*Y;
}

inline Vec2 vectorChangeOutOfCoordSys2(const Vec2 &v, const Vec2 &X, const Vec2 &Y){
	return v.a*X + v.b*Y;
}

inline Vec3 pointChangeIntoCoordSys3(const Vec3 &p, const Vec3 &O, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	Vec3 p_O(p - O);
	return Vec3(dot(p_O, X), dot(p_O, Y), dot(p_O, Z));
}

inline Vec2 pointChangeIntoCoordSys3(const Vec3 &p, const Vec3 &O, const Vec3 &X, const Vec3 &Y){
	Vec3 p_O(p - O);
	return Vec2(dot(p_O, X), dot(p_O, Y));
}


inline Vec3 vectorChangeIntoCoordSys3(const Vec3 &v, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	return Vec3(dot(v, X), dot(v, Y), dot(v, Z));
}

inline Vec2 vectorChangeIntoCoordSys3(const Vec3 &v, const Vec3 &X, const Vec3 &Y){
	return Vec2(dot(v, X), dot(v, Y));
}

inline Matrix33 matrixChangeIntoCoordSys3(const Matrix33 &m, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	Mat3 Q(X, Y, Z);
	Mat3 Qt(transpose(Q));
	return multiply(multiply(Qt, m), Q);
}

inline Matrix33 matrixChangeOutOfCoordSys3(const Matrix33 &m, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	Mat3 Q(X, Y, Z);
	Mat3 Qt(transpose(Q));
	return multiply(multiply(Q, m), Qt);
}

inline Vec3 pointChangeOutOfCoordSys3(const Vec3 &p, const Vec3 &O, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	return O + p.x*X + p.y*Y + p.z*Z;
}

inline Vec3 pointChangeOutOfCoordSys3(const Vec2 &p, const Vec3 &O, const Vec3 &X, const Vec3 &Y){
	return O + p.x*X + p.y*Y;
}


inline Vec3 vectorChangeOutOfCoordSys3(const Vec3 &v, const Vec3 &X, const Vec3 &Y, const Vec3 &Z){
	return v.a*X + v.b*Y + v.c*Z;
}

inline Vec3 vectorChangeOutOfCoordSys3(const Vec2 &v, const Vec3 &X, const Vec3 &Y){
	return v.a*X + v.b*Y;
}

inline void constructBasisFromXVector(const Vec3 &X_in, Vec3 &X_out, Vec3 &Y_out, Vec3 &Z_out){
	X_out.set(X_in);
	normalize(X_out);

	// generate vector vec not parallel to X_out, s.t. span(X_out, vec) is the XY-plane
	Vec3 vec(X_out);
	if(vec.x != 0)	{vec.x = 0; vec.y = 1;}
	else			{vec.x = 1; vec.y = 0;}

	// generate other axes
	Z_out.set(cross(X_out, vec));
	normalize(Z_out);
	Y_out.set(cross(Z_out, X_out));
}

inline void constructBasisFromZVector(const Vec3 &Z_in, Vec3 &X_out, Vec3 &Y_out, Vec3 &Z_out){
	Z_out.set(Z_in);
	normalize(Z_out);

	// generate vector vec not parallel to Z_out, s.t. span(Z_out, vec) is the ZX-plane
	Vec3 vec(Z_out);
	if(vec.x != 0)	{vec.x = 0; vec.y = 1;}
	else			{vec.x = 1; vec.y = 0;}

	// generate other axes
	Y_out.set(cross(Z_out, vec));
	normalize(Y_out);
	X_out.set(cross(Y_out, Z_out));
}


#endif

