#ifndef INC_Vector2_H
#define INC_Vector2_H

#include <Eigen/Dense>

#include <iomanip>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <adept.h>
using namespace std;

#include "stdafx.h"

class Vec2;
class Vec3;
class Vec4;

// Make Vec2 an Eigen expression to make it easier to use JPMath together with Eigen
namespace Eigen
{
	namespace internal
	{
		template<>
		struct traits<Vec2>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename Vector2d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit,
				RowsAtCompileTime = Vector2d::RowsAtCompileTime,
				ColsAtCompileTime = Vector2d::ColsAtCompileTime,
				MaxRowsAtCompileTime = Vector2d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = Vector2d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = RowsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit,
				Alignment = 0
			};
		};
	}
}

class Vec2 : public Eigen::MatrixBase<Vec2>
{
public:
	inline Vec2();
	Vec2(const Vec4 &a);
	Vec2(const Vec3 &a);
	explicit inline Vec2(double d);
	inline Vec2(double a0,double b0);
	inline Vec2(adept::adouble a0, adept::adouble b0);
	inline Vec2(const Vec2 &v);
	explicit inline Vec2(const double    *v);

	inline Vec2 &set(double a0,double b0);
	inline Vec2 &set(double d);
	inline Vec2 &set_x(double d);
	inline Vec2 &set_y(double d);


	inline Vec2 &set(const Vec2 &v);
	inline Vec2 &set(const double    *v);  

	inline Vec2 &operator=(double d);
	inline Vec2 &operator=(const Vec2 &v);
	inline Vec2 &operator=(const double *v);  

	inline int operator==(const Vec2 &v) const;
	inline int operator!=(const Vec2 &v) const;
	inline int operator==(double d) const;
	inline int operator!=(double d) const;

	inline Vec2 &operator+=(double d);
	inline Vec2 &operator-=(double d);
	inline Vec2 &operator*=(double d);
	inline Vec2 &operator/=(double d);

	inline Vec2 &operator+=(const Vec2 &v);
	inline Vec2 &operator-=(const Vec2 &v);
	inline Vec2 &operator*=(const Vec2 &v);
	inline Vec2 &operator/=(const Vec2 &v);
	
	inline double angle();

	inline Vec2 rotate90CCW();
	inline Vec2 rotate90CW();
	inline Vec2 rotate(double ang);
	
	inline double  dot(const Vec2 &v);

	union{
		struct{
			double a, b;
		};
		struct{
			double x, y;
		};
		double elements[2];
	};

	inline double length();

	typedef Eigen::Vector2d::Index Index;
	typedef Eigen::internal::ref_selector<Vec2>::type Nested;
	typedef Eigen::MatrixBase<Vec2> Base;

	double* data()
	{
		return &elements[0];
	}

	const double* data() const
	{
		return &elements[0];
	}

	template<typename OtherDerived>
	explicit Vec2(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec2(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec2(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Vec2& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec2& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec2& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 2;
	}

	Index cols() const
	{
		return 1;
	}

	Index innerStride() const
	{
		return 1;
	}

	Index outerStride() const
	{
		return 2;
	}
};

// Mostly copied from Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct evaluator<Vec2> : evaluator_base<Vec2>
		{
			typedef PlainObjectBase<Vector2d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Vec2 XprType;

			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Vec2>::EvaluatorFlags,
				Alignment = traits<Vec2>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Vec2& v) :
				m_data(v.data()) {}

			CoeffReturnType coeff(Index row, Index col) const
			{
				return m_data[row];
			}

			CoeffReturnType coeff(Index index) const
			{
				return m_data[index];
			}

			Scalar& coeffRef(Index row, Index col)
			{
				return const_cast<Scalar*>(m_data)[row];
			}

			Scalar& coeffRef(Index index)
			{
				return const_cast<Scalar*>(m_data)[index];
			}

			template<int LoadMode, typename PacketType>
			PacketType packet(Index row, Index col) const
			{
				return ploadt<PacketType, LoadMode>(m_data + row);
			}

			template<int LoadMode, typename PacketType>
			PacketType packet(Index index) const
			{
				return ploadt<PacketType, LoadMode>(m_data + index);
			}

			template<int StoreMode, typename PacketType>
			void writePacket(Index row, Index col, const PacketType& x)
			{
				return pstoret<Scalar, PacketType, StoreMode>(const_cast<Scalar*>(m_data) + row, x);
			}

			template<int StoreMode, typename PacketType>
			void writePacket(Index index, const PacketType& x)
			{
				return pstoret<Scalar, PacketType, StoreMode>(const_cast<Scalar*>(m_data) + index, x);
			}

		protected:
			const Scalar *m_data;
		};
	}
}

inline Vec2 operator-(const Vec2 &v);

inline Vec2 operator+(const Vec2 &v, const Vec2 &u);
inline Vec2 operator-(const Vec2 &v, const Vec2 &u);

inline Vec2 operator+(const Vec2 &v, double b);
inline Vec2 operator-(const Vec2 &v, double b);

inline Vec2 operator+(double a, const Vec2 &u);
inline Vec2 operator-(double a, const Vec2 &u);

inline double    length(const Vec2 &v);
inline double    lengthSq(const Vec2 &v);
inline void normalize(Vec2 &v);

inline Vec2 rotate(double angle, Vec2 &v);
inline Vec2 createVector(const double ang, double len = 1);


inline double   dot(const Vec2 &v,const Vec2 &u);
inline double   dotPerp(const Vec2 &v,const Vec2 &u);
inline Vec2 cross(const Vec2 &v);

inline Vec2::Vec2() {
  a = b = 0.0;
}

inline Vec2::Vec2(double d) {
  a = b = d;
}

inline Vec2::Vec2(double a0, double b0) {
  a = a0;
  b = b0;
}

inline Vec2::Vec2(adept::adouble a0, adept::adouble b0)
{
	a = a0.value();
	b = b0.value();
}

inline Vec2::Vec2(const Vec2 &v) {
  a = v.a;
  b = v.b;
}

inline Vec2::Vec2(const double *v) {
  a = v[0];
  b = v[1];
}
  
inline Vec2 &Vec2::set(double a0, double b0) {
  a = a0;
  b = b0;
  return (*this);
}

inline Vec2 &Vec2::set(double d) {
  a = d;
  b = d;
  return (*this);
}

inline Vec2 & Vec2::set_x(double d)
{
	a = d;
	return (*this);
}

inline Vec2 & Vec2::set_y(double d)
{
	b = d;
	return (*this);
}

inline Vec2 &Vec2::set(const Vec2 &v) {
  a = v.a;
  b = v.b;
  return (*this);
}
  
inline Vec2 &Vec2::set(const double *v) {
  a = v[0];
  b = v[1];
  return (*this);
}
  
inline Vec2 operator-(const Vec2 &v) {
  return Vec2(-v.a,-v.b);
}
 
inline Vec2 &Vec2::operator=(const Vec2 &v) {
  return set(v);
}

inline Vec2 &Vec2::operator=(double d) {
  return set(d);
}

inline int Vec2::operator==(const Vec2 &v) const {
  return ((a == v.a) &&
	  (b == v.b));
}

inline int Vec2::operator!=(const Vec2 &v) const {
  return ((a != v.a) ||
	  (b != v.b));
}

inline int Vec2::operator==(double d) const {
  return ((a == d) &&
	  (b == d));
}

inline int Vec2::operator!=(double d) const {
  return ((a != d) ||
	  (b != d));
}

inline Vec2 &Vec2::operator+=(double d) {
  a += d;
  b += d;
  return (*this);
}
    
inline Vec2 &Vec2::operator-=(double d) {
  a -= d;
  b -= d;
  return (*this);
}

inline Vec2 &Vec2::operator*=(double d) {
  a *= d;
  b *= d;
  return (*this);
}

inline Vec2 &Vec2::operator/=(double d) {
  a /= d;
  b /= d;
  return (*this);
}

inline Vec2 &Vec2::operator+=(const Vec2 &v) {
  a += v.a;
  b += v.b;
  return (*this);
}

inline Vec2 &Vec2::operator-=(const Vec2 &v) {
  a -= v.a;
  b -= v.b;
  return (*this);
}

inline Vec2 &Vec2::operator*=(const Vec2 &v) {
  a *= v.a;
  b *= v.b;
  return (*this);
}

inline double Vec2::angle(){
	return atan2(b, a);
}

inline Vec2 Vec2::rotate90CCW(){
	return Vec2(-b, a);
}


inline Vec2 Vec2::rotate90CW(){
	return Vec2(b, -a);
}

inline Vec2 Vec2::rotate(double ang){
	double ca = ::cos(ang);
	double sa = ::sin(ang);
	return Vec2(ca*a-sa*b, sa*a+ca*b);
}

inline double Vec2::dot(const Vec2 & v)
{
	return (v.a * a + v.b * b);
}

inline double Vec2::length()
{
	return std::sqrt(a*a + b*b);
}

inline Vec2 operator+(const Vec2 &v,const Vec2 &u) {
  return Vec2(v.a + u.a, v.b + u.b);
}

inline Vec2 operator-(const Vec2 &v,const Vec2 &u) {
  return Vec2(v.a - u.a, v.b - u.b);
}

inline Vec2 operator+(const Vec2 &v,double b){
  return Vec2(v.a + b, v.b + b);
}

inline Vec2 operator-(const Vec2 &v,double b){
  return Vec2(v.a - b, v.b - b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec2>::type operator*(const Vec2 &v, T b) {
	return Vec2(v.a * b, v.b * b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec2>::type operator/(const Vec2 &v, T b) {
	return Vec2(v.a / b, v.b / b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec2>::type operator*(T a, const Vec2 &b) {
	return b*a;
}

inline Vec2 operator+(double d, const Vec2 &u){
  return Vec2(d + u.a, d + u.b);
}

inline Vec2 operator-(double d,const Vec2 &u){
  return Vec2(d - u.a, d - u.b);
}

inline double length(const Vec2 &v) {
  return sqrt(v.a * v.a + v.b * v.b);
}

inline double lengthSq(const Vec2 &v) {
  return v.a * v.a + v.b * v.b;
}

inline Vec2 createVector(const double ang, double len) {
  return Vec2(len*cos(ang), len*cos(ang));
}

inline void normalize(Vec2 &v) {
  register double m = length(v);
  if (m != 0) v *= (1/m);
}

inline double dot(const Vec2 &v,const Vec2 &u) {
  return (v.a * u.a + v.b * u.b);
}

inline double dotPerp(const Vec2 &v,const Vec2 &u) {
  return (v.a * u.b - v.b * u.a);
}
  
inline Vec2 cross(const Vec2 &v) {
  return Vec2(-v.b, v.a);
}

inline Vec2 rotate(double angle, Vec2 &v){
	double ca = cos(angle);
	double sa = sin(angle);
	return Vec2(ca*v.a-sa*v.b, sa*v.a+ca*v.b);
}

inline ostream& operator<< (ostream &ostr, Vec2 &vec){
	ostr << "<" << vec.x << " " << vec.y << ">";
	return ostr;
}

inline istream& operator>> (istream &istr, Vec2 &vec){
	char pat[1024];
	istr >> setw(strlen("<")+1) >> pat >> vec.x >> vec.y >> setw(strlen(">")+1) >> pat;
	return istr;
}

#include "Vector4.h"

// Disambiguate mixed vector operations
inline Vec2 operator-(const Vec4 &v, const Vec2 &u) {
	return Vec2(v.a - u.a, v.b - u.b);
}
inline Vec2 operator-(const Vec2 &v, const Vec4 &u) {
	return Vec2(v.a - u.a, v.b - u.b);
}

#endif
