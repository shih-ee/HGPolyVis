#ifndef INC_Vector4_H
#define INC_Vector4_H

#include <Eigen/Dense>

#include <iomanip>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
using namespace std;

#include <math.h>

class Vec4;
class Vec3;
class Vec2;

// Make Vector4 an Eigen expression to make it easier to use JPMath together with Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct traits<Vec4>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename Vector4d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit,
				RowsAtCompileTime = Vector4d::RowsAtCompileTime,
				ColsAtCompileTime = Vector4d::ColsAtCompileTime,
				MaxRowsAtCompileTime = Vector4d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = Vector4d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = RowsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit,
				Alignment = 0
			};
		};
	}
}

class Vec4 : public Eigen::MatrixBase<Vec4>
{
public:
	inline Vec4();
	explicit inline Vec4(double v);
	inline Vec4(double a0, double b0, double c0, double d0);
	inline Vec4(const Vec4 &a);
	Vec4(const Vec3 &a, double b = 0);
	Vec4(const Vec2 &a, double b = 0, double c = 0);
	explicit inline Vec4(const double *a);

	inline Vec4 &set(double a0, double b0, double c0, double d0);

	inline Vec4 &set(const Vec4 &v);
	inline Vec4 &set(const double d);
	inline Vec4 &set(const double *v);  

	inline Vec4 &operator=(const Vec4 &v);
	inline Vec4 &operator=(const double d);
	inline Vec4 &operator=(const double *a);  

	inline int operator==(const Vec4 &v) const;
	inline int operator!=(const Vec4 &v) const;
	inline int operator==(double d) const;
	inline int operator!=(double d) const;

	inline Vec4 &operator+=(double f);
	inline Vec4 &operator-=(double f);
	inline Vec4 &operator*=(double f);
	inline Vec4 &operator/=(double f);

	inline Vec4 &operator+=(const Vec4 &v);
	inline Vec4 &operator-=(const Vec4 &v);
	inline Vec4 &operator*=(const Vec4 &v);
	inline Vec4 &operator/=(const Vec4 &v);
	inline double length(const Vec4 &v);
	inline void normalize(Vec4 &a);

	inline double dot(const Vec4 &v, const Vec4 &b);


	union{
		struct{
			double a, b, c, d;
		};
		struct{
			double x, y, z, w;
		};
		struct {
			double R, G, B, Alpha;
		};
		double elements[4];
	};

	typedef Eigen::Vector4d::Index Index;
	typedef Eigen::internal::ref_selector<Vec4>::type Nested;
	typedef Eigen::MatrixBase<Vec4> Base;

	double* data()
	{
		return &elements[0];
	}

	const double* data() const
	{
		return &elements[0];
	}

	template<typename OtherDerived>
	explicit Vec4(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec4(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec4(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Vec4& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec4& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec4& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 4;
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
		return 4;
	}
};

// Mostly copied from Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct evaluator<Vec4> : evaluator_base<Vec4>
		{
			typedef PlainObjectBase<Vector4d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Vec4 XprType;

			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Vec4>::EvaluatorFlags,
				Alignment = traits<Vec4>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Vec4& v) :
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

inline Vec4 operator-(const Vec4 &v);

inline Vec4 operator+(const Vec4 &v, const Vec4 &u);
inline Vec4 operator-(const Vec4 &v, const Vec4 &u);

inline Vec4 operator+(const Vec4 &v, double b);
inline Vec4 operator-(const Vec4 &v, double b);

inline Vec4 operator+(double a, const Vec4 &u);
inline Vec4 operator-(double a, const Vec4 &u);

inline double length(const Vec4 &v);
inline double lengthSq(const Vec4 &v);
inline void normalize(Vec4 &v);

inline double dot(const Vec4 &v,const Vec4 &u);
inline Vec4 cross(const Vec4 &v, const Vec4 &u);

inline Vec4 rotate(double angle, Vec4 axis, Vec4 v);

inline Vec4::Vec4() {
  a = b = c = 0.0;
}

inline Vec4::Vec4(double v) {
  a = b = c = d = v;
}

inline Vec4::Vec4(double a0,double b0, double c0, double d0) {
  a = a0;
  b = b0;
  c = c0;
  d = d0;
}

inline Vec4::Vec4(const Vec4 &v) {
  a = v.a;
  b = v.b;
  c = v.c;
  d = v.d;
}

inline Vec4::Vec4(const double *v) {
  a = v[0];
  b = v[1];
  c = v[2];
  d = v[3];
}
  
inline Vec4 &Vec4::set(double a0, double b0, double c0, double d0) {
  a = a0;
  b = b0;
  c = c0;
  d = d0;
  return (*this);
}

inline Vec4 &Vec4::set(double f) {
	a = b = c = d = f;
	return (*this);
}

inline Vec4 &Vec4::set(const Vec4 &v) {
  a = v.a;
  b = v.b;
  c = v.c;
  d = v.d;
  return (*this);
}
  
inline Vec4 &Vec4::set(const double *v) {
  a = v[0];
  b = v[1];
  c = v[2];
  d = v[3];
  return (*this);
}
  
inline Vec4 operator-(const Vec4 &v) {
  return Vec4(-v.a, -v.b, -v.c,  -v.d);
}

 
inline Vec4 &Vec4::operator=(const Vec4 &v) {
  return set(v);
}

inline Vec4 &Vec4::operator=(const double *v) {
  return set(v);
}

inline Vec4 &Vec4::operator=(const double d) {
  return set(d);
}

inline int Vec4::operator==(const Vec4 &v) const {
  return ((a == v.a) && (b == v.b) && (c == v.c) && (d == v.d));
}

inline int Vec4::operator!=(const Vec4 &v) const {
  return ((a != v.a) || (b != v.b) || (c != v.c) || (d != v.d));
}

inline int Vec4::operator==(double f) const {
  return ((a == f) && (b == f) && (c == f) && (d == f));
}

inline int Vec4::operator!=(double f) const {
  return ((a != f) || (b != f) || (c != f) || (d != f));
}

inline Vec4 &Vec4::operator+=(double f) {
  a += f;
  b += f;
  c += f;
  d += f;
  return (*this);
}
    
inline Vec4 &Vec4::operator-=(double f) {
  a -= f;
  b -= f;
  c -= f;
  d -= f;
  return (*this);
}

inline Vec4 &Vec4::operator*=(double f) {
  a *= f;
  b *= f;
  c *= f;
  d *= f;
  return (*this);
}

inline Vec4 &Vec4::operator+=(const Vec4 &v) {
  a += v.a;
  b += v.b;
  c += v.c;
  d += v.d;
  return (*this);
}

inline Vec4 &Vec4::operator-=(const Vec4 &v) {
  a -= v.a;
  b -= v.b;
  c -= v.c;
  d -= v.d;
  return (*this);
}

inline Vec4 &Vec4::operator*=(const Vec4 &v) {
  a *= v.a;
  b *= v.b;
  c *= v.c;
  d *= v.d;
  return (*this);
}

inline Vec4 operator+(const Vec4 &v,const Vec4 &u) {
  return Vec4(v.a + u.a, v.b + u.b, v.c + u.c, v.d + u.d);
}

inline Vec4 operator-(const Vec4 &v,const Vec4 &u) {
  return Vec4(v.a - u.a, v.b - u.b, v.c - u.c, v.d - u.d);
}

inline Vec4 operator+(const Vec4 &v,double b){
  return Vec4(v.a + b, v.b + b, v.c + b, v.d + b);
}

inline Vec4 operator-(const Vec4 &v,double b){
  return Vec4(v.a - b, v.b - b, v.c - b, v.d - b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec4>::type operator*(const Vec4 &v, T b) {
	return Vec4(v.a * b, v.b * b, v.c * b, v.d * b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec4>::type operator/(const Vec4 &v, T b) {
	return Vec4(v.a / b, v.b / b, v.c / b, v.d / b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec4>::type operator*(T a, const Vec4 &b) {
	return b*a;
}

inline Vec4 operator+(double a,const Vec4 &u){
  return Vec4(a + u.a, a + u.b, a + u.c, a + u.d);
}

inline Vec4 operator-(double a,const Vec4 &u){
  return Vec4(a - u.a, a - u.b, a - u.c, a - u.d);
}

inline double length(const Vec4 &v) {
  return sqrt(v.a * v.a + v.b * v.b + v.c * v.c + v.d * v.d);
}

inline double lengthSq(const Vec4 &v) {
  return v.a * v.a + v.b * v.b + v.c * v.c + v.d * v.d;
}

inline void normalize(Vec4 &v) {
  register double m = length(v);
  if (m != 0.0) v *= (1/m);
}

inline double dot(const Vec4 &v,const Vec4 &u) {
  return (v.a * u.a + v.b * u.b + v.c * u.c + v.d * u.d);
}

inline ostream& operator<< (ostream &ostr, Vec4 &vec){
	ostr << "<" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << ">";
	return ostr;
}

inline istream& operator>> (istream &istr, Vec4 &vec){
	char pat[1024];
	istr >> setw(strlen("<")+1) >> pat >> vec.x >> vec.y >> vec.z >>  vec.w >> setw(strlen(">")+1) >> pat;
	return istr;
}

typedef Vec4 Color4_8BIT;
typedef Vec4 Color4_F;

#endif
