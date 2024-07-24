/*

This file contains data structures that are used for matrix-related computatons.

Eugene Zhang  January 2005

*/

#ifndef INC_Matrix22_H
#define INC_Matrix22_H

#include "Vector2.h"
#include "JPMathConstants.h"
#include "JPComplex.h"

class Matrix22;
class Matrix33;

// Make Matrix22 an Eigen expression to make it easier to use JPMath together with Eigen.
namespace Eigen
{
	typedef Matrix<double, 2, 2, Eigen::RowMajor> RowMatrix2d;

	namespace internal
	{
		template<>
		struct traits<Matrix22>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename RowMatrix2d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit,
				RowsAtCompileTime = RowMatrix2d::RowsAtCompileTime,
				ColsAtCompileTime = RowMatrix2d::ColsAtCompileTime,
				MaxRowsAtCompileTime = RowMatrix2d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = RowMatrix2d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = ColsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit | RowMajorBit,
				Alignment = 0
			};
		};
	}
}

// start for class Matrix22
class Matrix22 : public Eigen::MatrixBase<Matrix22>
{
public:
  inline Matrix22();
  explicit inline Matrix22(double x);
  inline Matrix22(const double &diag00, const double &diag11);
  explicit inline Matrix22(const Vec2 &diag);
  inline Matrix22(const Matrix22 &that);
  inline Matrix22(const Vec2 &col0, const Vec2 &col1);

  inline Matrix22(double M00, double M01, 
		     double M10, double M11);
  explicit inline Matrix22(double M[2][2]);

  inline Matrix22 &set      (const double d);
  inline Matrix22 &set(double M00, double M01, 
		     double M10, double M11);
  inline Matrix22 &operator=(const double d);

  inline Matrix22 &set      (const Matrix22 &that);  
  inline Matrix22 &operator=(const Matrix22 &that); 

	inline Matrix22 &set			 (double M[2][2]);
  inline Matrix22 &operator=(double M[2][2]); 

  inline int operator!=(const Matrix22 &that)const; 
  inline int operator==(const Matrix22 &that)const; 

  inline int operator==(double d) const;
  inline int operator!=(double d) const;
  
  inline Matrix22 &operator+=(double d);
  inline Matrix22 &operator-=(double d);
  inline Matrix22 &operator*=(double d);

  // component-wise operations.
  inline Matrix22 &operator+=(const Matrix22 &that);
  inline Matrix22 &operator-=(const Matrix22 &that);

  // Left : this = that x this  
  // Right: this = this x that
  Matrix22 &leftMultiply (const Matrix22 &that);
  Matrix22 &rightMultiply(const Matrix22 &that);

  inline Matrix22 &setIdentity     ();

  inline void eigVecVals(double *e1, double *e2, Vec2 *ev1, Vec2 *ev2);
  inline void eigValsComplex(JPComplex eval[2]);
  inline void eigVecVals(double eval[2], Vec2 evec[2]);

public:
	union {
		double elements[2][2];
		struct {
			double m00, m01, m10, m11;
		};
	};

	typedef Eigen::RowMatrix2d::Index Index;
	typedef Eigen::internal::ref_selector<Matrix22>::type Nested;
	typedef Eigen::MatrixBase<Matrix22> Base;

	double* data()
	{
		return &elements[0][0];
	}

	const double* data() const
	{
		return &elements[0][0];
	}

	template<typename OtherDerived>
	explicit Matrix22(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Matrix22(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Matrix22(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Matrix22& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Matrix22& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Matrix22& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 2;
	}

	Index cols() const
	{
		return 2;
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
		struct evaluator<Matrix22> : evaluator_base<Matrix22>
		{
			typedef PlainObjectBase<RowMatrix2d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Matrix22 XprType;

			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Matrix22>::EvaluatorFlags,
				Alignment = traits<Matrix22>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Matrix22& m) :
				m_data(m.elements) {}

			CoeffReturnType coeff(Index row, Index col) const
			{
				return m_data[row][col];
			}

			CoeffReturnType coeff(Index index) const
			{
				return (&m_data[0][0])[index];
			}

			Scalar& coeffRef(Index row, Index col)
			{
				return const_cast<Scalar(*)[2]>(m_data)[row][col];
			}

			Scalar& coeffRef(Index index)
			{
				return (&const_cast<Scalar(*)[2]>(m_data)[0][0])[index];
			}

			template<int LoadMode, typename PacketType>
			PacketType packet(Index row, Index col) const
			{
				return ploadt<PacketType, LoadMode>(&m_data[row][col]);
			}

			template<int LoadMode, typename PacketType>
			PacketType packet(Index index) const
			{
				return ploadt<PacketType, LoadMode>(&m_data[0][0] + index);
			}

			template<int StoreMode, typename PacketType>
			void writePacket(Index row, Index col, const PacketType& x)
			{
				return pstoret<Scalar, PacketType, StoreMode>(const_cast<Scalar(*)[2]>(m_data)[row][col], x);
			}

			template<int StoreMode, typename PacketType>
			void writePacket(Index index, const PacketType& x)
			{
				return pstoret<Scalar, PacketType, StoreMode>(&const_cast<Scalar(*)[2]>(m_data)[0][0] + index, x);
			}

		protected:
			const Scalar(*m_data)[2];
		};
	}
}

inline Matrix22 operator-(const Matrix22 &a){
	 return Matrix22(-a.elements[0][0],-a.elements[0][1],
					 -a.elements[1][0],-a.elements[1][1]);
}

const Matrix22 dI22(1.0, 0.0, 0.0, 1.0);

inline Matrix22 operator+(const Matrix22 &a, double b);
inline Matrix22 operator-(const Matrix22 &a, double b);
inline Matrix22 operator+(const Matrix22 &a, const Matrix22 &b);
inline Matrix22 operator-(const Matrix22 &a, const Matrix22 &b);
inline Matrix22 operator*(const Matrix22 &a, const Matrix22 &b); 
inline Vec2  operator*(const Matrix22 &a, const Vec2   &b);
inline Vec2  operator*(const Vec2   &a, const Matrix22 &b);
inline Matrix22 multiply(const Matrix22 &a, const Matrix22 &b); 

inline double determinant(const Matrix22 &a);

inline Matrix22 transpose(const Matrix22 &a);
inline Matrix22   inverse(const Matrix22 &a);

inline Matrix22::Matrix22() {
  elements[0][0] = 1;
  elements[0][1] = 0;
  elements[1][0] = 0;
  elements[1][1] = 1;
}

inline Matrix22::Matrix22(double x) {
  elements[0][0] = x;
  elements[0][1] = x;
  elements[1][0] = x;
  elements[1][1] = x;
}

inline Matrix22::Matrix22(const double &diag00, const double &diag11){
	elements[0][0] = diag00;
	elements[0][1] = 0;
	elements[1][0] = 0;
	elements[1][1] = diag11;
}

inline Matrix22::Matrix22(const Vec2 &diag){
	elements[0][0] = diag.a;
	elements[0][1] = 0;
	elements[1][0] = 0;
	elements[1][1] = diag.b;
}

inline Matrix22::Matrix22(double M00, double M01, 
						  double M10, double M11) {
  elements[0][0] = M00;
  elements[0][1] = M01;
  elements[1][0] = M10;
  elements[1][1] = M11;
};

inline Matrix22::Matrix22(const Matrix22 &that) {
	elements[0][0] = that.elements[0][0];
	elements[0][1] = that.elements[0][1];
	elements[1][0] = that.elements[1][0];
	elements[1][1] = that.elements[1][1];
};

inline Matrix22::Matrix22(const Vec2 &col0, const Vec2 &col1){
	elements[0][0] = col0.x;
	elements[1][0] = col0.y;

	elements[0][1] = col1.x;
	elements[1][1] = col1.y;
}

inline Matrix22 &Matrix22::set(const double d) {
  return (*this)=d;
}

inline Matrix22 &Matrix22::set(double M00, double M01, 
						       double M10, double M11) {
  elements[0][0] = M00;
  elements[0][1] = M01;
  elements[1][0] = M10;
  elements[1][1] = M11;
  return (*this);
};

inline Matrix22 &Matrix22::operator=(const double d) {
  elements[0][0] = d;
  elements[0][1] = d;

  elements[1][0] = d;
  elements[1][1] = d;
  return (*this);
};

inline Matrix22 &Matrix22::set(const Matrix22 &that) {
  return (*this)=that;
}

inline Matrix22 &Matrix22::operator=(const Matrix22 &that) {
  elements[0][0] = that.elements[0][0];
  elements[0][1] = that.elements[0][1];

  elements[1][0] = that.elements[1][0];
  elements[1][1] = that.elements[1][1];
  return (*this);
};

inline Matrix22 &Matrix22::set(double M[2][2]) {
  return (*this)=M;
}

inline Matrix22 &Matrix22::operator=(double M[2][2]) {
  elements[0][0] = M[0][0];
  elements[0][1] = M[0][1];

  elements[1][0] = M[1][0];
  elements[1][1] = M[1][1];
  return (*this);
};

inline int Matrix22::operator==(double d) const {
  return  ( (elements[0][0] == d) &&
	    (elements[0][1] == d) &&
	    (elements[1][0] == d) &&
	    (elements[1][1] == d) );
}

inline int Matrix22::operator!=(double d) const {
  return  ( (elements[0][0] != d) ||
	    (elements[0][1] != d) ||
	    (elements[1][0] != d) ||
	    (elements[1][1] != d) );
}
  
inline int Matrix22::operator==(const Matrix22 &that)const {
  return ( (elements[0][0] == that.elements[0][0]) &&
	   (elements[0][1] == that.elements[0][1]) &&
	   (elements[1][0] == that.elements[1][0]) &&
	   (elements[1][1] == that.elements[1][1]) );
}

inline int Matrix22::operator!=(const Matrix22 &that)const {
  return ( (elements[0][0] != that.elements[0][0]) ||
	   (elements[0][1] != that.elements[0][1]) ||
	   (elements[1][0] != that.elements[1][0]) ||
	   (elements[1][1] != that.elements[1][1]) );
}

inline Matrix22 &Matrix22::operator+=(double d) {
  elements[0][0] += d; elements[1][0] += d; 
  elements[0][1] += d; elements[1][1] += d; 
  return (*this);
}

inline Matrix22 &Matrix22::operator-=(double d) {
  elements[0][0] -= d; elements[1][0] -= d; 
  elements[0][1] -= d; elements[1][1] -= d; 
  return (*this);
}

inline Matrix22 &Matrix22::operator*=(double d) {
  elements[0][0] *= d; elements[1][0] *= d; 
  elements[0][1] *= d; elements[1][1] *= d; 
  return (*this);
}

inline Matrix22 &Matrix22::operator+=(const Matrix22 &that) {
  elements[0][0] += that.elements[0][0]; elements[1][0] += that.elements[1][0]; 
  elements[0][1] += that.elements[0][1]; elements[1][1] += that.elements[1][1]; 
  return (*this);
}
  
inline Matrix22 &Matrix22::operator-=(const Matrix22 &that) {
  elements[0][0] -= that.elements[0][0]; elements[1][0] -= that.elements[1][0]; 
  elements[0][1] -= that.elements[0][1]; elements[1][1] -= that.elements[1][1]; 
  return (*this);
}

inline Matrix22 &Matrix22::leftMultiply (const Matrix22 &that){
	Matrix22 tmp(elements[0][0], elements[0][1], elements[1][0], elements[1][1]);
	
	elements[0][0] = that.elements[0][0] * tmp.elements[0][0] + that.elements[0][1] * tmp.elements[1][0];
	elements[0][1] = that.elements[0][0] * tmp.elements[0][1] + that.elements[0][1] * tmp.elements[1][1];
	elements[1][0] = that.elements[1][0] * tmp.elements[0][0] + that.elements[1][1] * tmp.elements[1][0];
	elements[1][1] = that.elements[1][0] * tmp.elements[0][1] + that.elements[1][1] * tmp.elements[1][1];
	return (*this);
};

inline Matrix22 &Matrix22::rightMultiply(const Matrix22 &that){
	Matrix22 tmp(elements[0][0], elements[0][1], elements[1][0], elements[1][1]);

	elements[0][0] = tmp.elements[0][0] * that.elements[0][0] + tmp.elements[0][1] * that.elements[1][0];
	elements[0][1] = tmp.elements[0][0] * that.elements[0][1] + tmp.elements[0][1] * that.elements[1][1];
	elements[1][0] = tmp.elements[1][0] * that.elements[0][0] + tmp.elements[1][1] * that.elements[1][0];
	elements[1][1] = tmp.elements[1][0] * that.elements[0][1] + tmp.elements[1][1] * that.elements[1][1];
	return (*this);
};

inline Matrix22 &Matrix22::setIdentity() {
  elements[0][0] = 1; elements[0][1] = 0; 
  elements[1][0] = 0; elements[1][1] = 1; 
  return (*this);
};

inline Matrix22 operator+(const Matrix22 &a,double b) {
  return (Matrix22(a)+=b);
}

inline Matrix22 operator-(const Matrix22 &a,double b) {
  return (Matrix22(a)-=b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Matrix22>::type operator*(const Matrix22 &a, T b) {
	return  Matrix22(a) *= b;
}

inline Matrix22 operator+(double a, const Matrix22 &b) {
return b+a;
}

inline Matrix22 operator-(double a, const Matrix22 &b) {
  return Matrix22(a-b.elements[0][0],a-b.elements[0][1],
		     a-b.elements[1][0],a-b.elements[1][1]);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Matrix22>::type operator*(T a, const Matrix22 &b) {
	return b*a;
}
 
inline Matrix22 operator+(const Matrix22 &a,const Matrix22 &b) {
  return (Matrix22(a)+=b);
}
 
inline Matrix22 operator-(const Matrix22 &a,const Matrix22 &b) {
  return (Matrix22(a)-=b);
}

inline Matrix22 operator*(const Matrix22 &a,const Matrix22 &b) {
  return  multiply(a,b);
}

inline Matrix22 multiply(const Matrix22 &a,const Matrix22 &b) {
  Matrix22 tmp(a);
  tmp.rightMultiply(b);
  return tmp;
}

inline Vec2 operator*(const Matrix22 &a,const Vec2 &b) {
  return Vec2(b.a*a.elements[0][0] + b.b*a.elements[0][1],
		   b.a*a.elements[1][0] + b.b*a.elements[1][1]);
}

inline Vec2 operator*(const Vec2 &a,const Matrix22 &b) {
  return Vec2(a.a*b.elements[0][0] + a.b*b.elements[1][0],
		   a.a*b.elements[0][1] + a.b*b.elements[1][1]);
}

inline double determinant(const Matrix22 &a) {
  return ( a.elements[0][0] * a.elements[1][1] - a.elements[0][1] * a.elements[1][0] );
}

inline Matrix22 transpose(const Matrix22 &a) {
  Matrix22 tmp(a);

	tmp.elements[0][1] = a.elements[1][0];
	tmp.elements[1][0] = a.elements[0][1];
  return tmp;
}

inline Matrix22 inverse(const Matrix22 &a) {
	Matrix22 tmp;
	double dmt;
	
	if ((dmt=determinant(a))!= 0.0) {
		tmp.elements[0][0] = a.elements[1][1]/dmt;
		tmp.elements[0][1] = -a.elements[0][1]/dmt;
		tmp.elements[1][0] = -a.elements[1][0]/dmt;
		tmp.elements[1][1] = a.elements[0][0]/dmt;
	}
	return tmp;
}

inline Matrix22 pow(const Matrix22 &m, int p) {
	Matrix22 tmp(m);
	
	if(p == 0)
		return Matrix22();
	
	for(int i = 1; i < p; i++)
		tmp = multiply(tmp, m);
	
	return tmp;
}

inline void Matrix22::eigVecVals(double eval[2], Vec2 evec[2]){
	double a = elements[0][0];
	double b = elements[0][1];
	double c = elements[1][0];
	double d = elements[1][1];
	
	double tr = a+d;
	double det = a*d - b*c;

	double disc = ::sqrt(tr*tr/4-det);
	eval[0] = tr*.5 + disc;
	eval[1] = tr*.5 - disc;

	if(fabs(c) > 10*EPS){
		evec[0].set(eval[0] - d, c);
		evec[1].set(eval[1] - d, c);
		::normalize(evec[0]);
		::normalize(evec[1]);
	}
	else if(fabs(b) > 10*EPS){
		evec[0].set(b, eval[0] - a);
		evec[1].set(b, eval[1] - a);
		::normalize(evec[0]);
		::normalize(evec[1]);
	}
	else{
		evec[0].set(1, 0);
		evec[1].set(0, 1);
	}
}



inline void Matrix22::eigValsComplex(JPComplex eval[2]){
	double a = elements[0][0];
	double b = elements[0][1];
	double c = elements[1][0];
	double d = elements[1][1];
	
	double tr = a+d;
	double det = a*d - b*c;

	JPComplex disc = sqrtToComplex(tr*tr/4-det);
	eval[0] = tr*.5 + disc;
	eval[1] = tr*.5 - disc;

}

inline void Matrix22::eigVecVals(double *e1, double *e2, Vec2 *ev1, Vec2 *ev2){
	double a = elements[0][0];
	double b = elements[0][1];
	double c = elements[1][0];
	double d = elements[1][1];
	
	double tr = a+d;
	double det = a*d - b*c;

	double disc = ::sqrt(tr*tr/4-det);
	*e1 = tr*.5 + disc;
	*e2 = tr*.5 - disc;

	if(fabs(c) > 10*EPS){
		ev1->set(*e1 - d, c);
		ev2->set(*e2 - d, c);
		::normalize(*ev1);
		::normalize(*ev2);
	}
	else if(fabs(b) > 10*EPS){
		ev1->set(b, *e1 - a);
		ev2->set(b, *e2 - a);
		::normalize(*ev1);
		::normalize(*ev2);
	}
	else{
		ev1->set(1, 0);
		ev2->set(0, 1);
	}

	
}

#endif
