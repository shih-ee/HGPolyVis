#ifndef INC_Vector5_H
#define INC_Vector5_H

#include <Eigen/Dense>

#include <iomanip>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
using namespace std;

#include <math.h>

class Vec5;
class Vec3;
class Vec2;

// Make Vector5 an Eigen expression to make it easier to use JPMath together with Eigen.
namespace Eigen
{
	typedef Matrix<double, 5, 1> Vector5d;

	namespace internal
	{
		template<>
		struct traits<Vec5>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename Vector5d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit,
				RowsAtCompileTime = Vector5d::RowsAtCompileTime,
				ColsAtCompileTime = Vector5d::ColsAtCompileTime,
				MaxRowsAtCompileTime = Vector5d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = Vector5d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = RowsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit,
				Alignment = 0
			};
		};
	}
}

class Vec5 : public Eigen::MatrixBase<Vec5>
{
public:
	inline Vec5();
	explicit inline Vec5(int n);
	explicit inline Vec5(double v);
	inline Vec5(double a0, double b0, double c0, double d0, double e0);
	inline Vec5(const Vec5 &a);
	Vec5(const Vec3 &a, double b, double c);
	Vec5(const Vec2 &a, double b, double c, double d);
	explicit inline Vec5(const double *a);

	inline Vec5 &set(double a0, double b0, double c0, double d0, double e0);

	inline Vec5 &set(const Vec5 &v);
	inline Vec5 &set(const double d);
	inline Vec5 &set(const double *v);

	inline Vec5 &operator=(const Vec5 &v);
	inline Vec5 &operator=(const double d);
	inline Vec5 &operator=(const double *a);

	inline int operator==(const Vec5 &v) const;
	inline int operator!=(const Vec5 &v) const;
	inline int operator==(double d) const;
	inline int operator!=(double d) const;

	inline Vec5 &operator+=(double f);
	inline Vec5 &operator-=(double f);
	inline Vec5 &operator*=(double f);
	inline Vec5 &operator/=(double f);

	inline Vec5 &operator+=(const Vec5 &v);
	inline Vec5 &operator-=(const Vec5 &v);
	inline Vec5 &operator*=(const Vec5 &v);
	inline Vec5 &operator/=(const Vec5 &v);
	inline double length(const Vec5 &v);
	inline void normalize(Vec5 &a);

	inline double dot(const Vec5 &v, const Vec5 &b);


	union {
		struct {
			double a, b, c, d, e;
		};
		struct {
			double x, y, z, w, q;
		};
		double elements[5];
	};

	typedef Eigen::Vector5d::Index Index;
	typedef Eigen::internal::ref_selector<Vec5>::type Nested;
	typedef Eigen::MatrixBase<Vec5> Base;

	double* data()
	{
		return &elements[0];
	}

	const double* data() const
	{
		return &elements[0];
	}

	template<typename OtherDerived>
	explicit Vec5(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec5(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Vec5(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Vec5& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec5& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Vec5& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 5;
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
		return 5;
	}
};

// Mostly copied from Eigen.
namespace Eigen
{
	namespace internal
	{
		template<>
		struct evaluator<Vec5> : evaluator_base<Vec5>
		{
			typedef PlainObjectBase<Vector5d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Vec5 XprType;

			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Vec5>::EvaluatorFlags,
				Alignment = traits<Vec5>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Vec5& v) :
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

inline Vec5 operator-(const Vec5 &v);

inline Vec5 operator+(const Vec5 &v, const Vec5 &u);
inline Vec5 operator-(const Vec5 &v, const Vec5 &u);

inline Vec5 operator+(const Vec5 &v, double b);
inline Vec5 operator-(const Vec5 &v, double b);

inline Vec5 operator+(double a, const Vec5 &u);
inline Vec5 operator-(double a, const Vec5 &u);

inline double length(const Vec5 &v);
inline double lengthSq(const Vec5 &v);
inline void normalize(Vec5 &v);

inline double dot(const Vec5 &v, const Vec5 &u);
inline Vec5 cross(const Vec5 &v, const Vec5 &u);
inline Vec5 rotate(double angle, Vec5 axis, Vec5 v);


inline Vec5::Vec5() {
	a = b = c = d = e = 0.0;
}

inline Vec5::Vec5(int n) {
	a = b = c =d = e= n;
}

inline Vec5::Vec5(double v) {
	a = b = c = d =e = v;
}

inline Vec5::Vec5(double a0, double b0, double c0, double d0, double e0) {
	a = a0;
	b = b0;
	c = c0;
	d = d0;
	e = e0;
}

inline Vec5::Vec5(const Vec5 &v) {
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;
	e = v.e;
}

inline Vec5::Vec5(const double *v) {
	a = v[0];
	b = v[1];
	c = v[2];
	d = v[3];
	e = v[4];
}

inline Vec5 &Vec5::set(double a0, double b0, double c0, double d0, double e0) {
	a = a0;
	b = b0;
	c = c0;
	d = d0;
	e = e0;
	return (*this);
}

inline Vec5 &Vec5::set(double f) {
	a = b = c = d = e = f;
	return (*this);
}

inline Vec5 &Vec5::set(const Vec5 &v) {
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;
	e = v.e;
	return (*this);
}

inline Vec5 &Vec5::set(const double *v) {
	a = v[0];
	b = v[1];
	c = v[2];
	d = v[3];
	e = v[4];
	return (*this);
}

inline Vec5 operator-(const Vec5 &v) {
	return Vec5(-v.a, -v.b, -v.c, -v.d, -v.e);
}


inline Vec5 &Vec5::operator=(const Vec5 &v) {
	return set(v);
}

inline Vec5 &Vec5::operator=(const double *v) {
	return set(v);
}

inline Vec5 &Vec5::operator=(const double d) {
	return set(d);
}

inline int Vec5::operator==(const Vec5 &v) const {
	return ((a == v.a) && (b == v.b) && (c == v.c) && (d == v.d) && (e == v.e));
}

inline int Vec5::operator!=(const Vec5 &v) const {
	return ((a != v.a) || (b != v.b) || (c != v.c) || (d != v.d) || (e != v.e));
}

inline int Vec5::operator==(double f) const {
	return ((a == f) && (b == f) && (c == f) && (d == f) && (e == f));
}

inline int Vec5::operator!=(double f) const {
	return ((a != f) || (b != f) || (c != f) || (d != f) || (e != f));
}

inline Vec5 &Vec5::operator+=(double f) {
	a += f;
	b += f;
	c += f;
	d += f;
	e += f;
	return (*this);
}

inline Vec5 &Vec5::operator-=(double f) {
	a -= f;
	b -= f;
	c -= f;
	d -= f;
	e -= f;
	return (*this);
}

inline Vec5 &Vec5::operator*=(double f) {
	a *= f;
	b *= f;
	c *= f;
	d *= f;
	e *= f;
	return (*this);
}

inline Vec5 &Vec5::operator+=(const Vec5 &v) {
	a += v.a;
	b += v.b;
	c += v.c;
	d += v.d;
	e += v.e;
	return (*this);
}

inline Vec5 &Vec5::operator-=(const Vec5 &v) {
	a -= v.a;
	b -= v.b;
	c -= v.c;
	d -= v.d;
	e -= v.e;
	return (*this);
}

inline Vec5 &Vec5::operator*=(const Vec5 &v) {
	a *= v.a;
	b *= v.b;
	c *= v.c;
	d *= v.d;
	e *= v.e;
	return (*this);
}

inline Vec5 operator+(const Vec5 &v, const Vec5 &u) {
	return Vec5(v.a + u.a, v.b + u.b, v.c + u.c, v.d + u.d, v.e + u.e);
}

inline Vec5 operator-(const Vec5 &v, const Vec5 &u) {
	return Vec5(v.a - u.a, v.b - u.b, v.c - u.c, v.d - u.d, v.e - u.e);
}

inline Vec5 operator+(const Vec5 &v, double b) {
	return Vec5(v.a + b, v.b + b, v.c + b, v.d + b, v.e + b);
}

inline Vec5 operator-(const Vec5 &v, double b) {
	return Vec5(v.a - b, v.b - b, v.c - b, v.d - b, v.e - b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec5>::type operator*(const Vec5 &v, T b) {
	return Vec5(v.a * b, v.b * b, v.c * b, v.d * b, v.e * b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec5>::type operator/(const Vec5 &v, T b) {
	return Vec5(v.a / b, v.b / b, v.c / b, v.d / b, v.e / b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Vec5>::type operator*(T a, const Vec5 &b) {
	return b*a;
}

inline Vec5 operator+(double a, const Vec5 &u) {
	return Vec5(a + u.a, a + u.b, a + u.c, a + u.d, a+u.e);
}

inline Vec5 operator-(double a, const Vec5 &u) {
	return Vec5(a - u.a, a - u.b, a - u.c, a - u.d, a - u.e);
}

inline double length(const Vec5 &v) {
	return sqrt(v.a * v.a + v.b * v.b + v.c * v.c + v.d * v.d + v.e * v.e);
}

inline double lengthSq(const Vec5 &v) {
	return v.a * v.a + v.b * v.b + v.c * v.c + v.d * v.d + v.e * v.e;
}

inline void normalize(Vec5 &v) {
	register double m = length(v);
	if (m != 0.0) v *= (1 / m);
}

inline double dot(const Vec5 &v, const Vec5 &u) {
	return (v.a * u.a + v.b * u.b + v.c * u.c + v.d * u.d + v.e * u.e);
}

inline ostream& operator<< (ostream &ostr, Vec5 &vec) {
	ostr << "<" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << " " << vec.q << ">";
	return ostr;
}

inline istream& operator >> (istream &istr, Vec5 &vec) {
	char pat[1024];
	istr >> setw(strlen("<") + 1) >> pat >> vec.x >> vec.y >> vec.z >> vec.w >> vec.q >> setw(strlen(">") + 1) >> pat;
	return istr;
}

double magnitude(const Vec5 &v);

#endif
