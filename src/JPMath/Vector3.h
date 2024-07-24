#ifndef INC_Vector3_H
#define INC_Vector3_H

#include <Eigen/Dense>

#include <iomanip>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <adept.h>

using namespace std;

class Vec4;
class Vec3;
class Vec2;

// Make Vec3 an Eigen expression to make it easier to use JPMath together with Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct traits<Vec3>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename Vector3d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit,
				RowsAtCompileTime = Vector3d::RowsAtCompileTime,
				ColsAtCompileTime = Vector3d::ColsAtCompileTime,
				MaxRowsAtCompileTime = Vector3d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = Vector3d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = RowsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit,
				Alignment = 0
			};
		};
	}
}

class Vec3 : public Eigen::MatrixBase<Vec3>
{
public:
	inline Vec3();
	explicit inline Vec3(double d);
	inline Vec3(const adept::adouble a0, const adept::adouble b0, const adept::adouble c0 = 0.0);
	inline Vec3(double a0, double b0, double c0 = 0.0);
	inline Vec3(const Vec3 &a);
	Vec3(const Vec4 &a);
	Vec3(const Vec2 &a, double b);
	explicit inline Vec3(const double *a);

	inline Vec3 &set(double a0, double b0,double c0);

	inline Vec3 &set(const Vec3 &v);
	inline Vec3 &set(const double d);
	inline Vec3 &set(const double *v);  

	inline Vec3 &operator=(const Vec3 &v);
	inline Vec3 &operator=(const double d);
	inline Vec3 &operator=(const double *a);  

	inline int operator==(const Vec3 &v) const;
	inline int operator!=(const Vec3 &v) const;
	inline int operator==(double d) const;
	inline int operator!=(double d) const;

	inline Vec3 &operator+=(double d);
	inline Vec3 &operator-=(double d);
	inline Vec3 &operator*=(double d);
	inline Vec3 &operator/=(double d);

	inline Vec3 &operator+=(const Vec3 &v);
	inline Vec3 &operator-=(const Vec3 &v);
	inline Vec3 &operator*=(const Vec3 &v);
	inline Vec3 &operator/=(const Vec3 &v);
	inline double length(const Vec3 &v);
	inline void normalize(Vec3 &a);

	inline Vec3 setMinTo1();

	inline double dot(const Vec3 &v, const Vec3 &b);
	inline Vec3 cross(const Vec3 &v);

	//string toString();

	union{
		struct{
			double a, b, c;
		};
		struct{
			double x, y, z;
		};
		struct {
			double R, G, B;
		};
		double elements[3];
	};

	typedef Eigen::Vector3d::Index Index;
	typedef Eigen::internal::ref_selector<Vec3>::type Nested;
	typedef Eigen::MatrixBase<Vec3> Base;

	double* data()
	{
		return &elements[0];
	}

	const double* data() const
	{
		return &elements[0];
	}

	template<typename OtherDerived>
	explicit Vec3(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec3(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec3(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Vec3& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec3& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec3& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 3;
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
		return 3;
	}
};

// Mostly copied from Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct evaluator<Vec3> : evaluator_base<Vec3>
		{
			typedef PlainObjectBase<Vector3d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Vec3 XprType;

			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Vec3>::EvaluatorFlags,
				Alignment = traits<Vec3>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Vec3& v) :
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

inline Vec3 operator-(const Vec3 &v);

inline Vec3 operator+(const Vec3 &v, const Vec3 &u);
inline Vec3 operator-(const Vec3 &v, const Vec3 &u);

inline Vec3 operator+(const Vec3 &v, double b);
inline Vec3 operator-(const Vec3 &v, double b);

inline Vec3 operator+(double a, const Vec3 &u);
inline Vec3 operator-(double a, const Vec3 &u);

inline double length(const Vec3 &v);
inline double lengthSq(const Vec3 &v);
inline void normalize(Vec3 &v);

inline double dot(const Vec3 &v,const Vec3 &u);
inline Vec3 cross(const Vec3 &v, const Vec3 &u);

inline Vec3 rotate(double angle, Vec3 axis, Vec3 v);

inline ostream& operator<< (ostream &ostr, Vec3 &vec);

inline Vec3::Vec3() {
  a = b = c = 0.0;
}

inline Vec3::Vec3(double d) {
  a = b = c = d;
}

inline Vec3::Vec3(const adept::adouble a0, const adept::adouble b0, const adept::adouble c0)
{
	a = a0.value();
	b = b0.value();
	c = c0.value();
}

inline Vec3::Vec3(double a0,double b0, double c0) {
  a = a0;
  b = b0;
  c = c0;
}

inline Vec3::Vec3(const Vec3 &v) {
  a = v.a;
  b = v.b;
  c = v.c;
}

inline Vec3::Vec3(const double *v) {
  a = v[0];
  b = v[1];
  c = v[2];
}

inline Vec3 &Vec3::set(double a0, double b0, double c0) {
  a = a0;
  b = b0;
  c = c0;
  return (*this);
}

inline Vec3 &Vec3::set(double d) {
	a = b = c = d;
	return (*this);
}

inline Vec3 &Vec3::set(const Vec3 &v) {
  a = v.a;
  b = v.b;
  c = v.c;
  return (*this);
}
  
inline Vec3 &Vec3::set(const double *v) {
  a = v[0];
  b = v[1];
  c = v[2];
  return (*this);
}
  
inline Vec3 operator-(const Vec3 &v) {
  return Vec3(-v.a, -v.b, -v.c);
}

 
inline Vec3 &Vec3::operator=(const Vec3 &v) {
  return set(v);
}

inline Vec3 &Vec3::operator=(const double *v) {
  return set(v);
}

inline Vec3 &Vec3::operator=(const double d) {
  return set(d);
}
  
inline Vec3 Vec3::setMinTo1(){
	Vec3 tmp(*this);

	double min = a;
	int i = 0;
	if(b < min){
		min = b;
		i = 1;
	}
	if(c < min){
		min = c;
		i = 2;
	}

	if(i == 0)
		tmp.a = 1.0;
	else if(i == 1)
		tmp.b = 1.0;
	else if(i == 2)
		tmp.c = 1.0;

	return tmp;
}

inline int Vec3::operator==(const Vec3 &v) const {
  return ((a == v.a) && (b == v.b) && (c == v.c));
}

inline int Vec3::operator!=(const Vec3 &v) const {
  return ((a != v.a) || (b != v.b) || (c != v.c));
}

inline int Vec3::operator==(double d) const {
  return ((a == d) && (b == d) && (c == d));
}

inline int Vec3::operator!=(double d) const {
  return ((a != d) || (b != d) || (c != d));
}

inline Vec3 &Vec3::operator+=(double d) {
  a += d;
  b += d;
  c += d;
  return (*this);
}
    
inline Vec3 &Vec3::operator-=(double d) {
  a -= d;
  b -= d;
  c -= d;
  return (*this);
}

inline Vec3 &Vec3::operator*=(double d) {
  a *= d;
  b *= d;
  c *= d;
  return (*this);
}

inline Vec3 &Vec3::operator/=(double d) {
  a /= d;
  b /= d;
  c /= d;
  return (*this);
}

inline Vec3 &Vec3::operator+=(const Vec3 &v) {
  a += v.a;
  b += v.b;
  c += v.c;
  return (*this);
}

inline Vec3 &Vec3::operator-=(const Vec3 &v) {
  a -= v.a;
  b -= v.b;
  c -= v.c;
  return (*this);
}

inline Vec3 &Vec3::operator*=(const Vec3 &v) {
  a *= v.a;
  b *= v.b;
  c *= v.c;
  return (*this);
}

inline Vec3 operator+(const Vec3 &v,const Vec3 &u) {
  return Vec3(v.a + u.a, v.b + u.b, v.c + u.c);
}

inline Vec3 operator-(const Vec3 &v,const Vec3 &u) {
	return Vec3(v.a - u.a, v.b - u.b, v.c - u.c);
}

inline Vec3 operator+(const Vec3 &v,double b){
  return Vec3(v.a + b, v.b + b, v.c + b);
}

inline Vec3 operator-(const Vec3 &v,double b){
  return Vec3(v.a - b, v.b - b, v.c - b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec3>::type operator*(const Vec3 &v, T b) {
	return Vec3(v.a * b, v.b * b, v.c * b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec3>::type operator/(const Vec3 &v, T b) {
	return Vec3(v.a / b, v.b / b, v.c / b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec3>::type operator*(T a, const Vec3 &b) {
	return b*a;
}

inline Vec3 operator+(double a,const Vec3 &u){
  return Vec3(a + u.a, a + u.b, a + u.c);
}

inline Vec3 operator-(double a,const Vec3 &u){
  return Vec3(a - u.a, a - u.b, a - u.c);
}

inline double length(const Vec3 &v) {
  return sqrt(v.a * v.a + v.b * v.b + v.c * v.c);
}

inline double lengthSq(const Vec3 &v) {
  return v.a * v.a + v.b * v.b + v.c * v.c;
}

inline void normalize(Vec3 &v) {
  register double m = length(v);
  if (m != 0.0) v *= (1/m);
}

inline double dot(const Vec3 &v,const Vec3 &u) {
  return (v.a * u.a + v.b * u.b + v.c * u.c);
}
  
inline Vec3 cross(const Vec3 &v, const Vec3 &u) {
	Vec3 result(v.b * u.c - v.c * u.b, 
				 v.c * u.a - v.a * u.c,
				 v.a * u.b - v.b * u.a);
	return result;
}

inline Vec3 rotate(double angle, Vec3 axis, Vec3 v){
	return Vec3(0.0);
}

inline ostream& operator<< (ostream &ostr, Vec3 &vec){
	ostr << "<" << vec.x << " " << vec.y << " " << vec.z << ">";
	return ostr;
}

inline istream& operator>> (istream &istr, Vec3 &vec){
	char pat[1024];
	istr >> setw(strlen("<")+1) >> pat >> vec.x >> vec.y >> vec.z >> setw(strlen(">")+1) >> pat;
	return istr;
}

#include "Vector2.h"
#include "Vector4.h"

// Disambiguate mixed vector operations
inline Vec2 operator-(const Vec3 &v, const Vec2 &u) {
	return Vec2(v.a - u.a, v.b - u.b);
}
inline Vec2 operator-(const Vec2 &v, const Vec3 &u) {
	return Vec2(v.a - u.a, v.b - u.b);
}
inline Vec3 operator-(const Vec3 &v, const Vec4 &u) {
	return Vec3(v.a - u.a, v.b - u.b, v.c - u.c);
}
inline Vec3 operator-(const Vec4 &v, const Vec3 &u) {
	return Vec3(v.a - u.a, v.b - u.b, v.c - u.c);
}

#endif
