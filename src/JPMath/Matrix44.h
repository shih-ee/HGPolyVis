#ifndef INC_MATRIX33_H
#define INC_MATRIX33_H

#include "Vector4.h"
#include "Vector3.h"

class Matrix44;

// Make Matrix44 an Eigen expression to make it easier to use JPMath together with Eigen.
namespace Eigen
{
	typedef Matrix<double, 4, 4, Eigen::RowMajor> RowMatrix4d;

	namespace internal
	{
		template<>
		struct traits<Matrix44>
		{
			typedef Dense StorageKind;
			typedef MatrixXpr XprKind;
			typedef typename RowMatrix4d::StorageIndex StorageIndex;
			typedef double Scalar;

			enum
			{
				Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit,
				RowsAtCompileTime = RowMatrix4d::RowsAtCompileTime,
				ColsAtCompileTime = RowMatrix4d::ColsAtCompileTime,
				MaxRowsAtCompileTime = RowMatrix4d::MaxRowsAtCompileTime,
				MaxColsAtCompileTime = RowMatrix4d::MaxColsAtCompileTime,

				InnerStrideAtCompileTime = 1,
				OuterStrideAtCompileTime = ColsAtCompileTime,

				EvaluatorFlags = LinearAccessBit | DirectAccessBit | RowMajorBit,
				Alignment = 0
			};
		};
	}
}

class Matrix44 : public Eigen::MatrixBase<Matrix44>
{
public:
	  inline Matrix44();
	  explicit inline Matrix44(double x);
	  inline Matrix44(const double &diag00, const double &diag11, const double &diag22, const double &diag33);
	  explicit inline Matrix44(const Vec4 &diag);
	  inline Matrix44(const Matrix44 &that);
	  inline Matrix44(const Vec4 &v0, const Vec4 &v1, const Vec4 &v2, const Vec4 &v3);

	  inline Matrix44(double M00, double M01, double M02, double M03,
					   double M10, double M11, double M12, double M13,
					   double M20, double M21, double M22, double M23,
					   double M30, double M31, double M32, double M33);

	  explicit inline Matrix44(double M[4][4]);

	  explicit inline Matrix44(double M[16]);

	  inline Matrix44 &set      (const double d);
	  inline Matrix44 &operator=(const double d);

	  inline Matrix44 &set      (const Matrix44 &that);  
	  inline Matrix44 &operator=(const Matrix44 &that); 

	  inline Matrix44 &set		(double M[4][4]);
	  inline Matrix44 &set		(double M[16]);
	  inline Matrix44 &operator=(double M[4][4]); 
	  inline Matrix44 &operator=(double M[16]);

	  inline Matrix44 &set(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3);
	  inline Matrix44 &set(double M00, double M01, double M02, double M03,
							double M10, double M11, double M12, double M13,
							double M20, double M21, double M22, double M23,
							double M30, double M31, double M32, double M33);

	  inline Matrix44 &pow(int p);

	  inline int operator!=(const Matrix44 &that)const; 
	  inline int operator==(const Matrix44 &that)const; 

	  inline int operator==(double d) const;
	  inline int operator!=(double d) const;
	  
	  inline Matrix44 &operator+=(double d);
	  inline Matrix44 &operator-=(double d);
	  inline Matrix44 &operator*=(double d);

	  // component-wise operations.
	  inline Matrix44 &operator+=(const Matrix44 &that);
	  inline Matrix44 &operator-=(const Matrix44 &that);

	  // Left : this = that x this  
	  // Right: this = this x that
	  Matrix44 &leftMultiply (const Matrix44 &that);
	  Matrix44 &rightMultiply(const Matrix44 &that);

	  inline Matrix44 &setIdentity();

	  inline void writeMatrix();

public:
	union{
		double elements[4][4];
		struct{
			double m00, m01, m02, m03,
			       m10, m11, m12, m13,
			       m20, m21, m22, m23,
			       m30, m31, m32, m33;
		};
	};

	typedef Eigen::RowMatrix4d::Index Index;
	typedef Eigen::internal::ref_selector<Matrix44>::type Nested;
	typedef Eigen::MatrixBase<Matrix44> Base;

	double* data()
	{
		return &elements[0][0];
	}

	const double* data() const
	{
		return &elements[0][0];
	}

	template<typename OtherDerived>
	explicit Matrix44(const Eigen::DenseBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Matrix44(const Eigen::EigenBase<OtherDerived> &other)
	{
		*this = other.derived();
	}

	template<typename OtherDerived>
	explicit Matrix44(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		other.evalTo(*this);
	}

	template <typename OtherDerived>
	Matrix44& operator=(const Eigen::DenseBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Matrix44& operator=(const Eigen::EigenBase<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	template <typename OtherDerived>
	Matrix44& operator=(const Eigen::ReturnByValue<OtherDerived>& other)
	{
		return Base::operator=(other);
	}

	Index rows() const
	{
		return 4;
	}

	Index cols() const
	{
		return 4;
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
		struct evaluator<Matrix44> : evaluator_base<Matrix44>
		{
			typedef PlainObjectBase<RowMatrix4d> PlainObjectType;
			typedef typename PlainObjectType::Scalar Scalar;
			typedef typename PlainObjectType::CoeffReturnType CoeffReturnType;

			typedef Matrix44 XprType;
            
			enum {
				IsRowMajor = PlainObjectType::IsRowMajor,
				IsVectorAtCompileTime = PlainObjectType::IsVectorAtCompileTime,
				RowsAtCompileTime = PlainObjectType::RowsAtCompileTime,
				ColsAtCompileTime = PlainObjectType::ColsAtCompileTime,

				CoeffReadCost = NumTraits<Scalar>::ReadCost,
				Flags = traits<Matrix44>::EvaluatorFlags,
				Alignment = traits<Matrix44>::Alignment
			};

			evaluator() : m_data(0) {}

			explicit evaluator(const Matrix44& m) :
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
				return const_cast<Scalar(*)[4]>(m_data)[row][col];
			}

			Scalar& coeffRef(Index index)
			{
				return (&const_cast<Scalar(*)[4]>(m_data)[0][0])[index];
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
				return pstoret<Scalar, PacketType, StoreMode>(const_cast<Scalar(*)[4]>(m_data)[row][col], x);
			}

			template<int StoreMode, typename PacketType>
			void writePacket(Index index, const PacketType& x)
			{
				return pstoret<Scalar, PacketType, StoreMode>(&const_cast<Scalar(*)[4]>(m_data)[0][0] + index, x);
			}

		protected:
			const Scalar (*m_data)[4];
		};
	}
}

inline Matrix44 operator-(const Matrix44 &a){
	 return Matrix44(-a.elements[0][0],-a.elements[0][1],-a.elements[0][2],-a.elements[0][3],
					 -a.elements[1][0],-a.elements[1][1],-a.elements[1][2],-a.elements[1][3],
					 -a.elements[2][0],-a.elements[2][1],-a.elements[2][2],-a.elements[2][3],
					 -a.elements[3][0],-a.elements[3][1],-a.elements[3][2],-a.elements[3][3]);
}

inline Matrix44 operator+(const Matrix44 &a, double b);
inline Matrix44 operator-(const Matrix44 &a, double b);
inline Matrix44 operator+(const Matrix44 &a, const Matrix44 &b);
inline Matrix44 operator-(const Matrix44 &a, const Matrix44 &b);
inline Matrix44 operator*(const Matrix44 &a, const Matrix44 &b); 
inline Matrix44 multiply(const Matrix44 &a, const Matrix44 &b); 
inline Matrix44 conjugate(const Matrix44 &a, const Matrix44 &b); 
inline Matrix44 othoconjugate(const Matrix44 &a, const Matrix44 &b); 
inline Vec4   operator*(const Matrix44 &a, const Vec4   &b);
inline Vec4   operator*(const Vec4   &a, const Matrix44 &b);

inline double determinant(const Matrix44 &a);

inline Matrix44 transpose(const Matrix44 &a);
inline Matrix44   inverse(const Matrix44 &a);
inline Matrix44 rotMat4(Vec3 v, double angle);

inline Matrix44::Matrix44() {
  elements[0][0] = 1;
  elements[0][1] = 0;
  elements[0][2] = 0;
  elements[0][3] = 0;
  elements[1][0] = 0;
  elements[1][1] = 1;
  elements[1][2] = 0;
  elements[1][3] = 0;
  elements[2][0] = 0;
  elements[2][1] = 0;
  elements[2][2] = 1;
  elements[2][3] = 0;
  elements[3][0] = 0;
  elements[3][1] = 0;
  elements[3][2] = 0;
  elements[3][3] = 1;
}

inline Matrix44::Matrix44(double x) {
  elements[0][0] = x;
  elements[0][1] = x;
  elements[0][2] = x;
  elements[0][3] = x;
  elements[1][0] = x;
  elements[1][1] = x;
  elements[1][2] = x;
  elements[1][3] = x;
  elements[2][0] = x;
  elements[2][1] = x;
  elements[2][2] = x;
  elements[2][3] = x;
  elements[3][0] = x;
  elements[3][1] = x;
  elements[3][2] = x;
  elements[3][3] = x;
}

inline Matrix44::Matrix44(const double &diag00, const double &diag11, const double &diag22, const double &diag33){
	elements[0][0] = diag00;
	elements[0][1] = 0;
	elements[0][2] = 0;
	elements[0][3] = 0;
	elements[1][0] = 0;
	elements[1][1] = diag11;
	elements[1][2] = 0;
	elements[1][3] = 0;
	elements[2][0] = 0;
	elements[2][1] = 0;
	elements[2][2] = diag22;
	elements[2][3] = 0;
	elements[3][0] = 0;
	elements[3][1] = 0;
	elements[3][2] = 0;
	elements[3][3] = diag33;
}

inline Matrix44::Matrix44(const Vec4 &diag){
	elements[0][0] = diag.x;
	elements[0][1] = 0;
	elements[0][2] = 0;
	elements[0][3] = 0;
	elements[1][0] = 0;
	elements[1][1] = diag.y;
	elements[1][2] = 0;
	elements[1][3] = 0;
	elements[2][0] = 0;
	elements[2][1] = 0;
	elements[2][2] = diag.z;
	elements[2][3] = 0;
	elements[3][0] = 0;
	elements[3][1] = 0;
	elements[3][2] = 0;
	elements[3][3] = diag.w;
}

inline Matrix44::Matrix44(double M00, double M01, double M02, double M03,
							double M10, double M11, double M12, double M13,
							double M20, double M21, double M22, double M23,
							double M30, double M31, double M32, double M33) {
  elements[0][0] = M00;
  elements[0][1] = M01;
  elements[0][2] = M02;
  elements[0][3] = M03;
  elements[1][0] = M10;
  elements[1][1] = M11;
  elements[1][2] = M12;
  elements[1][3] = M13;
  elements[2][0] = M20;
  elements[2][1] = M21;
  elements[2][2] = M22;
  elements[2][3] = M23;
  elements[3][0] = M30;
  elements[3][1] = M31;
  elements[3][2] = M32;
  elements[3][3] = M33;
}

inline Matrix44::Matrix44(const Matrix44 &that) {
  elements[0][0] = that.elements[0][0];
  elements[0][1] = that.elements[0][1];
  elements[0][2] = that.elements[0][2];
  elements[0][3] = that.elements[0][3];
  elements[1][0] = that.elements[1][0];
  elements[1][1] = that.elements[1][1];
  elements[1][2] = that.elements[1][2];
  elements[1][3] = that.elements[1][3];
  elements[2][0] = that.elements[2][0];
  elements[2][1] = that.elements[2][1];
  elements[2][2] = that.elements[2][2];
  elements[2][3] = that.elements[2][3];
  elements[3][0] = that.elements[3][0];
  elements[3][1] = that.elements[3][1];
  elements[3][2] = that.elements[3][2];
  elements[3][3] = that.elements[3][3];
}

inline Matrix44::Matrix44(double M[16]) {
  elements[0][0] = M[0];
  elements[1][0] = M[1];
  elements[2][0] = M[2];
  elements[3][0] = M[3];

  elements[0][1] = M[4];
  elements[1][1] = M[5];
  elements[2][1] = M[6];
  elements[3][1] = M[7];

  elements[0][2] = M[8];
  elements[1][2] = M[9];
  elements[2][2] = M[10];
  elements[3][2] = M[11];

  elements[0][3] = M[12];
  elements[1][3] = M[13];
  elements[2][3] = M[14];
  elements[3][3] = M[15];
}

inline Matrix44 &Matrix44::set(const double d) {
  return (*this)=d;
}

inline Matrix44 &Matrix44::operator=(const double d) {
  elements[0][0] = d;
  elements[0][1] = d;
  elements[0][2] = d;
  elements[0][3] = d;

  elements[1][0] = d;
  elements[1][1] = d;
  elements[1][2] = d;
  elements[1][3] = d;

  elements[2][0] = d;
  elements[2][1] = d;
  elements[2][2] = d;
  elements[2][3] = d;

  elements[3][0] = d;
  elements[3][1] = d;
  elements[3][2] = d;
  elements[3][3] = d;

  return (*this);
}

inline Matrix44 &Matrix44::set(const Matrix44 &that) {
  return (*this)=that;
}


inline Matrix44 &Matrix44::operator=(const Matrix44 &that) {
  elements[0][0] = that.elements[0][0];
  elements[0][1] = that.elements[0][1];
  elements[0][2] = that.elements[0][2];
  elements[0][3] = that.elements[0][3];
  elements[1][0] = that.elements[1][0];
  elements[1][1] = that.elements[1][1];
  elements[1][2] = that.elements[1][2];
  elements[1][3] = that.elements[1][3];
  elements[2][0] = that.elements[2][0];
  elements[2][1] = that.elements[2][1];
  elements[2][2] = that.elements[2][2];
  elements[2][3] = that.elements[2][3];
  elements[3][0] = that.elements[3][0];
  elements[3][1] = that.elements[3][1];
  elements[3][2] = that.elements[3][2];
  elements[3][3] = that.elements[3][3];
  return (*this);
}

inline Matrix44 &Matrix44::set(double M[4][4]) {
  return (*this)=M;
}

inline Matrix44 &Matrix44::operator=(double M[16]) {
  elements[0][0] = M[0];
  elements[1][0] = M[1];
  elements[2][0] = M[2];
  elements[3][0] = M[3];

  elements[0][1] = M[4];
  elements[1][1] = M[5];
  elements[2][1] = M[6];
  elements[3][1] = M[7];

  elements[0][2] = M[8];
  elements[1][2] = M[9];
  elements[2][2] = M[10];
  elements[3][2] = M[11];

  elements[0][3] = M[12];
  elements[1][3] = M[13];
  elements[2][3] = M[14];
  elements[3][3] = M[15];

return (*this);
}

inline Matrix44 &Matrix44::set(double M[16]) {
  return (*this)=M;
}

inline Matrix44 &Matrix44::operator=(double M[4][4]) {
  elements[0][0] = M[0][0];
  elements[0][1] = M[0][1];
  elements[0][2] = M[0][2];
  elements[0][3] = M[0][3];

  elements[1][0] = M[1][0];
  elements[1][1] = M[1][1];
  elements[1][2] = M[1][2];
  elements[1][3] = M[1][3];

  elements[2][0] = M[2][0];
  elements[2][1] = M[2][1];
  elements[2][2] = M[2][2];
  elements[2][3] = M[2][3];

  elements[3][0] = M[3][0];
  elements[3][1] = M[3][1];
  elements[3][2] = M[3][2];
  elements[3][3] = M[3][3];

return (*this);
}


inline Matrix44 &Matrix44::set(double M00, double M01, double M02, double M03,
								 double M10, double M11, double M12, double M13,
								 double M20, double M21, double M22, double M23,
								 double M30, double M31, double M32, double M33) {
  elements[0][0] = M00;
  elements[0][1] = M01;
  elements[0][2] = M02;
  elements[0][3] = M03;
  elements[1][0] = M10;
  elements[1][1] = M11;
  elements[1][2] = M12;
  elements[1][3] = M13;
  elements[2][0] = M20;
  elements[2][1] = M21;
  elements[2][2] = M22;
  elements[2][3] = M23;
  elements[3][0] = M30;
  elements[3][1] = M31;
  elements[3][2] = M32;
  elements[3][3] = M33;
	return (*this);
}

inline int Matrix44::operator==(double d) const {
  return  ( (elements[0][0] == d) && (elements[0][1] == d) && (elements[0][2] == d) && (elements[0][3] == d) &&
			(elements[1][0] == d) && (elements[1][1] == d) && (elements[1][2] == d) && (elements[1][3] == d) &&
			(elements[2][0] == d) && (elements[2][1] == d) && (elements[2][2] == d) && (elements[2][3] == d) &&
			(elements[3][0] == d) && (elements[3][1] == d) && (elements[3][2] == d) && (elements[3][3] == d) );
}

inline int Matrix44::operator!=(double d) const {
  return  ( (elements[0][0] != d) || (elements[0][1] != d) || (elements[0][2] != d) || (elements[0][3] != d) ||
			(elements[1][0] != d) || (elements[1][1] != d) || (elements[1][2] != d) || (elements[1][3] != d) ||
			(elements[2][0] != d) || (elements[2][1] != d) || (elements[2][2] != d) || (elements[2][3] != d) ||
			(elements[3][0] != d) || (elements[3][1] != d) || (elements[3][2] != d) || (elements[3][3] != d));
}
  
inline int Matrix44::operator==(const Matrix44 &that)const {
  return ( (elements[0][0] == that.elements[0][0]) && (elements[0][1] == that.elements[0][1]) && (elements[0][2] == that.elements[0][2]) && (elements[0][3] == that.elements[0][3]) &&
		   (elements[1][0] == that.elements[1][0]) && (elements[1][1] == that.elements[1][1]) && (elements[1][2] == that.elements[1][2]) && (elements[1][3] == that.elements[1][3]) &&
		   (elements[2][0] == that.elements[2][0]) && (elements[2][1] == that.elements[2][1]) && (elements[2][2] == that.elements[2][2]) && (elements[2][3] == that.elements[2][3]) &&
		   (elements[3][0] == that.elements[3][0]) && (elements[3][1] == that.elements[3][1]) && (elements[3][2] == that.elements[3][2]) && (elements[3][3] == that.elements[3][3]));
}

inline int Matrix44::operator!=(const Matrix44 &that)const {
  return ( (elements[0][0] != that.elements[0][0]) || (elements[0][1] != that.elements[0][1]) || (elements[0][2] != that.elements[0][2]) || (elements[0][3] != that.elements[0][3]) ||
		   (elements[1][0] != that.elements[1][0]) || (elements[1][1] != that.elements[1][1]) || (elements[1][2] != that.elements[1][2]) || (elements[1][3] != that.elements[1][3]) ||
		   (elements[2][0] != that.elements[2][0]) || (elements[2][1] != that.elements[2][1]) || (elements[2][2] != that.elements[2][2]) || (elements[2][3] != that.elements[2][3]) ||
		   (elements[3][0] != that.elements[3][0]) || (elements[3][1] != that.elements[3][1]) || (elements[3][2] != that.elements[3][2]) || (elements[3][3] != that.elements[3][3]));
}

inline Matrix44 &Matrix44::operator+=(double d) {
  elements[0][0] += d; elements[0][1] += d; elements[0][2] += d; elements[0][3] += d;
  elements[1][0] += d; elements[1][1] += d; elements[1][2] += d; elements[1][3] += d;
  elements[2][0] += d; elements[2][1] += d; elements[2][2] += d; elements[2][3] += d;
  elements[3][0] += d; elements[3][1] += d; elements[3][2] += d; elements[3][3] += d;
  return (*this);
}

inline Matrix44 &Matrix44::operator-=(double d) {
  elements[0][0] -= d; elements[0][1] -= d; elements[0][2] -= d; elements[0][3] -= d;
  elements[1][0] -= d; elements[1][1] -= d; elements[1][2] -= d; elements[1][3] -= d;
  elements[2][0] -= d; elements[2][1] -= d; elements[2][2] -= d; elements[2][3] -= d;
  elements[3][0] -= d; elements[3][1] -= d; elements[3][2] -= d; elements[3][3] -= d;
  return (*this);
}

inline Matrix44 &Matrix44::operator*=(double d) {
  elements[0][0] *= d; elements[0][1] *= d; elements[0][2] *= d; elements[0][3] *= d;
  elements[1][0] *= d; elements[1][1] *= d; elements[1][2] *= d; elements[1][3] *= d; 
  elements[2][0] *= d; elements[2][1] *= d; elements[2][2] *= d; elements[2][3] *= d; 
  elements[3][0] *= d; elements[3][1] *= d; elements[3][2] *= d; elements[3][3] *= d; 
  return (*this);
}

inline Matrix44 &Matrix44::operator+=(const Matrix44 &that) {
  elements[0][0] += that.elements[0][0]; elements[0][1] += that.elements[0][1]; elements[0][2] += that.elements[0][2]; elements[0][3] += that.elements[0][3];
  elements[1][0] += that.elements[1][0]; elements[1][1] += that.elements[1][1]; elements[1][2] += that.elements[1][2]; elements[1][3] += that.elements[1][3];
  elements[2][0] += that.elements[2][0]; elements[2][1] += that.elements[2][1]; elements[2][2] += that.elements[2][2]; elements[2][3] += that.elements[2][3];
  elements[3][0] += that.elements[3][0]; elements[3][1] += that.elements[2][1]; elements[3][2] += that.elements[3][2]; elements[3][3] += that.elements[3][3];
  return (*this);
}
  
inline Matrix44 &Matrix44::operator-=(const Matrix44 &that) {
  elements[0][0] -= that.elements[0][0]; elements[0][1] -= that.elements[0][1]; elements[0][2] -= that.elements[0][2]; elements[0][3] -= that.elements[0][3];
  elements[1][0] -= that.elements[1][0]; elements[1][1] -= that.elements[1][1]; elements[1][2] -= that.elements[1][2]; elements[1][3] -= that.elements[1][3];
  elements[2][0] -= that.elements[2][0]; elements[2][1] -= that.elements[2][1]; elements[2][2] -= that.elements[2][2]; elements[2][3] -= that.elements[2][3];
  elements[3][0] -= that.elements[3][0]; elements[3][1] -= that.elements[2][1]; elements[3][2] -= that.elements[3][2]; elements[3][3] -= that.elements[3][3];
  return (*this);
}

inline Matrix44 &Matrix44::leftMultiply (const Matrix44 &that){
	Matrix44 tmp(*this);
	
	elements[0][0] = that.elements[0][0] * tmp.elements[0][0] + that.elements[0][1] * tmp.elements[1][0] + that.elements[0][2] * tmp.elements[2][0] + that.elements[0][3] * tmp.elements[3][0];
	elements[0][1] = that.elements[0][0] * tmp.elements[0][1] + that.elements[0][1] * tmp.elements[1][1] + that.elements[0][2] * tmp.elements[2][1] + that.elements[0][3] * tmp.elements[3][1];
	elements[0][2] = that.elements[0][0] * tmp.elements[0][2] + that.elements[0][1] * tmp.elements[1][2] + that.elements[0][2] * tmp.elements[2][2] + that.elements[0][3] * tmp.elements[3][2];
	elements[0][3] = that.elements[0][0] * tmp.elements[0][3] + that.elements[0][1] * tmp.elements[1][3] + that.elements[0][2] * tmp.elements[2][3] + that.elements[0][3] * tmp.elements[3][3];

	elements[1][0] = that.elements[1][0] * tmp.elements[0][0] + that.elements[1][1] * tmp.elements[1][0] + that.elements[1][2] * tmp.elements[2][0] + that.elements[1][3] * tmp.elements[3][0];
	elements[1][1] = that.elements[1][0] * tmp.elements[0][1] + that.elements[1][1] * tmp.elements[1][1] + that.elements[1][2] * tmp.elements[2][1] + that.elements[1][3] * tmp.elements[3][1];
	elements[1][2] = that.elements[1][0] * tmp.elements[0][2] + that.elements[1][1] * tmp.elements[1][2] + that.elements[1][2] * tmp.elements[2][2] + that.elements[1][3] * tmp.elements[3][2];
	elements[1][3] = that.elements[1][0] * tmp.elements[0][3] + that.elements[1][1] * tmp.elements[1][3] + that.elements[1][2] * tmp.elements[2][3] + that.elements[1][3] * tmp.elements[3][3];

	elements[2][0] = that.elements[2][0] * tmp.elements[0][0] + that.elements[2][1] * tmp.elements[1][0] + that.elements[2][2] * tmp.elements[2][0] + that.elements[2][3] * tmp.elements[3][0];
	elements[2][1] = that.elements[2][0] * tmp.elements[0][1] + that.elements[2][1] * tmp.elements[1][1] + that.elements[2][2] * tmp.elements[2][1] + that.elements[2][3] * tmp.elements[3][1];
	elements[2][2] = that.elements[2][0] * tmp.elements[0][2] + that.elements[2][1] * tmp.elements[1][2] + that.elements[2][2] * tmp.elements[2][2] + that.elements[2][3] * tmp.elements[3][2];
	elements[2][3] = that.elements[2][0] * tmp.elements[0][3] + that.elements[2][1] * tmp.elements[1][3] + that.elements[2][2] * tmp.elements[2][3] + that.elements[2][3] * tmp.elements[3][3];

	elements[3][0] = that.elements[3][0] * tmp.elements[0][0] + that.elements[3][1] * tmp.elements[1][0] + that.elements[3][2] * tmp.elements[2][0] + that.elements[3][3] * tmp.elements[3][0];
	elements[3][1] = that.elements[3][0] * tmp.elements[0][1] + that.elements[3][1] * tmp.elements[1][1] + that.elements[3][2] * tmp.elements[2][1] + that.elements[3][3] * tmp.elements[3][1];
	elements[3][2] = that.elements[3][0] * tmp.elements[0][2] + that.elements[3][1] * tmp.elements[1][2] + that.elements[3][2] * tmp.elements[2][2] + that.elements[3][3] * tmp.elements[3][2];
	elements[3][3] = that.elements[3][0] * tmp.elements[0][3] + that.elements[3][1] * tmp.elements[1][3] + that.elements[3][2] * tmp.elements[2][3] + that.elements[3][3] * tmp.elements[3][3];
	return (*this);
};

inline Matrix44 &Matrix44::rightMultiply(const Matrix44 &that){
	Matrix44 tmp(*this);

	elements[0][0] = tmp.elements[0][0] * that.elements[0][0] + tmp.elements[0][1] * that.elements[1][0] + tmp.elements[0][2] * that.elements[2][0] + tmp.elements[0][3] * that.elements[3][0];
	elements[0][1] = tmp.elements[0][0] * that.elements[0][1] + tmp.elements[0][1] * that.elements[1][1] + tmp.elements[0][2] * that.elements[2][1] + tmp.elements[0][3] * that.elements[3][1];
	elements[0][2] = tmp.elements[0][0] * that.elements[0][2] + tmp.elements[0][1] * that.elements[1][2] + tmp.elements[0][2] * that.elements[2][2] + tmp.elements[0][3] * that.elements[3][2];
	elements[0][3] = tmp.elements[0][0] * that.elements[0][3] + tmp.elements[0][1] * that.elements[1][3] + tmp.elements[0][2] * that.elements[2][3] + tmp.elements[0][3] * that.elements[3][3];

	elements[1][0] = tmp.elements[1][0] * that.elements[0][0] + tmp.elements[1][1] * that.elements[1][0] + tmp.elements[1][2] * that.elements[2][0] + tmp.elements[1][3] * that.elements[3][0];
	elements[1][1] = tmp.elements[1][0] * that.elements[0][1] + tmp.elements[1][1] * that.elements[1][1] + tmp.elements[1][2] * that.elements[2][1] + tmp.elements[1][3] * that.elements[3][1];
	elements[1][2] = tmp.elements[1][0] * that.elements[0][2] + tmp.elements[1][1] * that.elements[1][2] + tmp.elements[1][2] * that.elements[2][2] + tmp.elements[1][3] * that.elements[3][2];
	elements[1][3] = tmp.elements[1][0] * that.elements[0][3] + tmp.elements[1][1] * that.elements[1][3] + tmp.elements[1][2] * that.elements[2][3] + tmp.elements[1][3] * that.elements[3][3];

	elements[2][0] = tmp.elements[2][0] * that.elements[0][0] + tmp.elements[2][1] * that.elements[1][0] + tmp.elements[2][2] * that.elements[2][0] + tmp.elements[2][3] * that.elements[3][0];
	elements[2][1] = tmp.elements[2][0] * that.elements[0][1] + tmp.elements[2][1] * that.elements[1][1] + tmp.elements[2][2] * that.elements[2][1] + tmp.elements[2][3] * that.elements[3][1];
	elements[2][2] = tmp.elements[2][0] * that.elements[0][2] + tmp.elements[2][1] * that.elements[1][2] + tmp.elements[2][2] * that.elements[2][2] + tmp.elements[2][3] * that.elements[3][2];
	elements[2][3] = tmp.elements[2][0] * that.elements[0][3] + tmp.elements[2][1] * that.elements[1][3] + tmp.elements[2][2] * that.elements[2][3] + tmp.elements[2][3] * that.elements[3][3];

	elements[3][0] = tmp.elements[3][0] * that.elements[0][0] + tmp.elements[3][1] * that.elements[1][0] + tmp.elements[3][2] * that.elements[2][0] + tmp.elements[3][3] * that.elements[3][0];
	elements[3][1] = tmp.elements[3][0] * that.elements[0][1] + tmp.elements[3][1] * that.elements[1][1] + tmp.elements[3][2] * that.elements[2][1] + tmp.elements[3][3] * that.elements[3][1];
	elements[3][2] = tmp.elements[3][0] * that.elements[0][2] + tmp.elements[3][1] * that.elements[1][2] + tmp.elements[3][2] * that.elements[2][2] + tmp.elements[3][3] * that.elements[3][2];
	elements[3][3] = tmp.elements[3][0] * that.elements[0][3] + tmp.elements[3][1] * that.elements[1][3] + tmp.elements[3][2] * that.elements[2][3] + tmp.elements[3][3] * that.elements[3][3];

	return (*this);
};

inline Matrix44 &Matrix44::setIdentity() {
  elements[0][0] = 1; elements[0][1] = 0; elements[0][2] = 0; elements[0][3] = 0;
  elements[1][0] = 0; elements[1][1] = 1; elements[1][2] = 0; elements[1][3] = 0;
  elements[2][0] = 0; elements[2][1] = 0; elements[2][2] = 1; elements[2][3] = 0;
  elements[3][0] = 0; elements[3][1] = 0; elements[3][2] = 0; elements[3][3] = 1;
  return (*this);
};


inline Matrix44 operator+(const Matrix44 &a,double b) {
  return (Matrix44(a)+=b);
}

inline Matrix44 operator-(const Matrix44 &a,double b) {
  return (Matrix44(a)-=b);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Matrix44>::type operator*(const Matrix44 &a, T b) {
	return  Matrix44(a) *= b;
}
 
inline Matrix44 operator+(double a, const Matrix44 &b) {
return b+a;
}

inline Matrix44 operator-(double a, const Matrix44 &b) {
  return Matrix44(a-b.elements[0][0],a-b.elements[0][1],a-b.elements[0][2],a-b.elements[0][3],
				   a-b.elements[1][0],a-b.elements[1][1],a-b.elements[1][2],a-b.elements[1][3],
				   a-b.elements[2][0],a-b.elements[2][1],a-b.elements[2][2],a-b.elements[2][3],
				   a-b.elements[3][0],a-b.elements[3][1],a-b.elements[3][2],a-b.elements[3][3]);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, Matrix44>::type operator*(T a, const Matrix44 &b) {
	return b*a;
}
 
inline Matrix44 operator+(const Matrix44 &a,const Matrix44 &b) {
  return (Matrix44(a)+=b);
}
 
inline Matrix44 operator-(const Matrix44 &a,const Matrix44 &b) {
  return (Matrix44(a)-=b);
}

inline Matrix44 operator*(const Matrix44 &a,const Matrix44 &b) {
  return  multiply(a,b);
}

inline Matrix44 multiply(const Matrix44 &a,const Matrix44 &b) {
  Matrix44 tmp(a);
  tmp.rightMultiply(b);
  return tmp;
}

inline Matrix44 conjugate(const Matrix44 a, const Matrix44 &b) {
  Matrix44 tmp(a);
	Matrix44 c = inverse(b);
  tmp.rightMultiply(b);
	tmp.leftMultiply(c);
  return tmp;
}

inline Matrix44 othoconjugate(const Matrix44 &a, const Matrix44 &b) {
  Matrix44 tmp(a);
	Matrix44 c = transpose(b);
  tmp.rightMultiply(b);
	tmp.leftMultiply(c);
  return tmp;
}

inline Vec4 operator*(const Matrix44 &a,const Vec4 &b) {
  return Vec4(b.a*a.elements[0][0] + b.b*a.elements[0][1] + b.c*a.elements[0][2] + b.d*a.elements[0][3], 
				  b.a*a.elements[1][0] + b.b*a.elements[1][1] + b.c*a.elements[1][2] + b.d*a.elements[1][3],
				  b.a*a.elements[2][0] + b.b*a.elements[2][1] + b.c*a.elements[2][2] + b.d*a.elements[2][3],
				  b.a*a.elements[3][0] + b.b*a.elements[3][1] + b.c*a.elements[3][2] + b.d*a.elements[3][3]);
}

inline Vec4 operator*(const Vec4 &a,const Matrix44 &b) {
  return Vec4(a.a*b.elements[0][0] + a.b*b.elements[1][0] + a.c*b.elements[2][0] + a.d*b.elements[3][0],
				  a.a*b.elements[0][1] + a.b*b.elements[1][1] + a.c*b.elements[2][1] + a.d*b.elements[3][1],
				  a.a*b.elements[0][2] + a.b*b.elements[1][2] + a.c*b.elements[2][2] + a.d*b.elements[3][2],
				  a.a*b.elements[0][3] + a.b*b.elements[1][3] + a.c*b.elements[2][3] + a.d*b.elements[3][3]);
}

inline Matrix44 transpose(const Matrix44 &a) {
  Matrix44 tmp;

  for(int i = 0; i < 4; i++){
	  for(int j = 0; j < 4; j++){
		  tmp.elements[j][i] = a.elements[i][j];
	  }
  }

  return tmp;
}


inline Matrix44 inverse(const Matrix44 &a) {
	Matrix44 tmp;
	tmp.elements[0][0] = a.elements[1][2]*a.elements[2][3]*a.elements[3][1] - a.elements[1][3]*a.elements[2][2]*a.elements[3][1] + a.elements[1][3]*a.elements[2][1]*a.elements[3][2] - a.elements[1][1]*a.elements[2][3]*a.elements[3][2] - a.elements[1][2]*a.elements[2][1]*a.elements[3][3] + a.elements[1][1]*a.elements[2][2]*a.elements[3][3];
	tmp.elements[0][1] = a.elements[0][3]*a.elements[2][2]*a.elements[3][1] - a.elements[0][2]*a.elements[2][3]*a.elements[3][1] - a.elements[0][3]*a.elements[2][1]*a.elements[3][2] + a.elements[0][1]*a.elements[2][3]*a.elements[3][2] + a.elements[0][2]*a.elements[2][1]*a.elements[3][3] - a.elements[0][1]*a.elements[2][2]*a.elements[3][3];
	tmp.elements[0][2] = a.elements[0][2]*a.elements[1][3]*a.elements[3][1] - a.elements[0][3]*a.elements[1][2]*a.elements[3][1] + a.elements[0][3]*a.elements[1][1]*a.elements[3][2] - a.elements[0][1]*a.elements[1][3]*a.elements[3][2] - a.elements[0][2]*a.elements[1][1]*a.elements[3][3] + a.elements[0][1]*a.elements[1][2]*a.elements[3][3];
	tmp.elements[0][3] = a.elements[0][3]*a.elements[1][2]*a.elements[2][1] - a.elements[0][2]*a.elements[1][3]*a.elements[2][1] - a.elements[0][3]*a.elements[1][1]*a.elements[2][2] + a.elements[0][1]*a.elements[1][3]*a.elements[2][2] + a.elements[0][2]*a.elements[1][1]*a.elements[2][3] - a.elements[0][1]*a.elements[1][2]*a.elements[2][3];
	tmp.elements[1][0] = a.elements[1][3]*a.elements[2][2]*a.elements[3][0] - a.elements[1][2]*a.elements[2][3]*a.elements[3][0] - a.elements[1][3]*a.elements[2][0]*a.elements[3][2] + a.elements[1][0]*a.elements[2][3]*a.elements[3][2] + a.elements[1][2]*a.elements[2][0]*a.elements[3][3] - a.elements[1][0]*a.elements[2][2]*a.elements[3][3];
	tmp.elements[1][1] = a.elements[0][2]*a.elements[2][3]*a.elements[3][0] - a.elements[0][3]*a.elements[2][2]*a.elements[3][0] + a.elements[0][3]*a.elements[2][0]*a.elements[3][2] - a.elements[0][0]*a.elements[2][3]*a.elements[3][2] - a.elements[0][2]*a.elements[2][0]*a.elements[3][3] + a.elements[0][0]*a.elements[2][2]*a.elements[3][3];
	tmp.elements[1][2] = a.elements[0][3]*a.elements[1][2]*a.elements[3][0] - a.elements[0][2]*a.elements[1][3]*a.elements[3][0] - a.elements[0][3]*a.elements[1][0]*a.elements[3][2] + a.elements[0][0]*a.elements[1][3]*a.elements[3][2] + a.elements[0][2]*a.elements[1][0]*a.elements[3][3] - a.elements[0][0]*a.elements[1][2]*a.elements[3][3];
	tmp.elements[1][3] = a.elements[0][2]*a.elements[1][3]*a.elements[2][0] - a.elements[0][3]*a.elements[1][2]*a.elements[2][0] + a.elements[0][3]*a.elements[1][0]*a.elements[2][2] - a.elements[0][0]*a.elements[1][3]*a.elements[2][2] - a.elements[0][2]*a.elements[1][0]*a.elements[2][3] + a.elements[0][0]*a.elements[1][2]*a.elements[2][3];
	tmp.elements[2][0] = a.elements[1][1]*a.elements[2][3]*a.elements[3][0] - a.elements[1][3]*a.elements[2][1]*a.elements[3][0] + a.elements[1][3]*a.elements[2][0]*a.elements[3][1] - a.elements[1][0]*a.elements[2][3]*a.elements[3][1] - a.elements[1][1]*a.elements[2][0]*a.elements[3][3] + a.elements[1][0]*a.elements[2][1]*a.elements[3][3];
	tmp.elements[2][1] = a.elements[0][3]*a.elements[2][1]*a.elements[3][0] - a.elements[0][1]*a.elements[2][3]*a.elements[3][0] - a.elements[0][3]*a.elements[2][0]*a.elements[3][1] + a.elements[0][0]*a.elements[2][3]*a.elements[3][1] + a.elements[0][1]*a.elements[2][0]*a.elements[3][3] - a.elements[0][0]*a.elements[2][1]*a.elements[3][3];
	tmp.elements[2][2] = a.elements[0][1]*a.elements[1][3]*a.elements[3][0] - a.elements[0][3]*a.elements[1][1]*a.elements[3][0] + a.elements[0][3]*a.elements[1][0]*a.elements[3][1] - a.elements[0][0]*a.elements[1][3]*a.elements[3][1] - a.elements[0][1]*a.elements[1][0]*a.elements[3][3] + a.elements[0][0]*a.elements[1][1]*a.elements[3][3];
	tmp.elements[2][3] = a.elements[0][3]*a.elements[1][1]*a.elements[2][0] - a.elements[0][1]*a.elements[1][3]*a.elements[2][0] - a.elements[0][3]*a.elements[1][0]*a.elements[2][1] + a.elements[0][0]*a.elements[1][3]*a.elements[2][1] + a.elements[0][1]*a.elements[1][0]*a.elements[2][3] - a.elements[0][0]*a.elements[1][1]*a.elements[2][3];
	tmp.elements[3][0] = a.elements[1][2]*a.elements[2][1]*a.elements[3][0] - a.elements[1][1]*a.elements[2][2]*a.elements[3][0] - a.elements[1][2]*a.elements[2][0]*a.elements[3][1] + a.elements[1][0]*a.elements[2][2]*a.elements[3][1] + a.elements[1][1]*a.elements[2][0]*a.elements[3][2] - a.elements[1][0]*a.elements[2][1]*a.elements[3][2];
	tmp.elements[3][1] = a.elements[0][1]*a.elements[2][2]*a.elements[3][0] - a.elements[0][2]*a.elements[2][1]*a.elements[3][0] + a.elements[0][2]*a.elements[2][0]*a.elements[3][1] - a.elements[0][0]*a.elements[2][2]*a.elements[3][1] - a.elements[0][1]*a.elements[2][0]*a.elements[3][2] + a.elements[0][0]*a.elements[2][1]*a.elements[3][2];
	tmp.elements[3][2] = a.elements[0][2]*a.elements[1][1]*a.elements[3][0] - a.elements[0][1]*a.elements[1][2]*a.elements[3][0] - a.elements[0][2]*a.elements[1][0]*a.elements[3][1] + a.elements[0][0]*a.elements[1][2]*a.elements[3][1] + a.elements[0][1]*a.elements[1][0]*a.elements[3][2] - a.elements[0][0]*a.elements[1][1]*a.elements[3][2];
	tmp.elements[3][3] = a.elements[0][1]*a.elements[1][2]*a.elements[2][0] - a.elements[0][2]*a.elements[1][1]*a.elements[2][0] + a.elements[0][2]*a.elements[1][0]*a.elements[2][1] - a.elements[0][0]*a.elements[1][2]*a.elements[2][1] - a.elements[0][1]*a.elements[1][0]*a.elements[2][2] + a.elements[0][0]*a.elements[1][1]*a.elements[2][2];
	tmp *= 1/determinant(a);

	return tmp;
}



inline double determinant(const Matrix44 &a) {
	double value;
	value =
	a.elements[0][3] * a.elements[1][2] * a.elements[2][1] * a.elements[3][0]-a.elements[0][2] * a.elements[1][3] * a.elements[2][1] * a.elements[3][0]-a.elements[0][3] * a.elements[1][1] * a.elements[2][2] * a.elements[3][0]+a.elements[0][1] * a.elements[1][3]    * a.elements[2][2] * a.elements[3][0]+
	a.elements[0][2] * a.elements[1][1] * a.elements[2][3] * a.elements[3][0]-a.elements[0][1] * a.elements[1][2] * a.elements[2][3] * a.elements[3][0]-a.elements[0][3] * a.elements[1][2] * a.elements[2][0] * a.elements[3][1]+a.elements[0][2] * a.elements[1][3]    * a.elements[2][0] * a.elements[3][1]+
	a.elements[0][3] * a.elements[1][0] * a.elements[2][2] * a.elements[3][1]-a.elements[0][0] * a.elements[1][3] * a.elements[2][2] * a.elements[3][1]-a.elements[0][2] * a.elements[1][0] * a.elements[2][3] * a.elements[3][1]+a.elements[0][0] * a.elements[1][2]    * a.elements[2][3] * a.elements[3][1]+
	a.elements[0][3] * a.elements[1][1] * a.elements[2][0] * a.elements[3][2]-a.elements[0][1] * a.elements[1][3] * a.elements[2][0] * a.elements[3][2]-a.elements[0][3] * a.elements[1][0] * a.elements[2][1] * a.elements[3][2]+a.elements[0][0] * a.elements[1][3]    * a.elements[2][1] * a.elements[3][2]+
	a.elements[0][1] * a.elements[1][0] * a.elements[2][3] * a.elements[3][2]-a.elements[0][0] * a.elements[1][1] * a.elements[2][3] * a.elements[3][2]-a.elements[0][2] * a.elements[1][1] * a.elements[2][0] * a.elements[3][3]+a.elements[0][1] * a.elements[1][2]    * a.elements[2][0] * a.elements[3][3]+
	a.elements[0][2] * a.elements[1][0] * a.elements[2][1] * a.elements[3][3]-a.elements[0][0] * a.elements[1][2] * a.elements[2][1] * a.elements[3][3]-a.elements[0][1] * a.elements[1][0] * a.elements[2][2] * a.elements[3][3]+a.elements[0][0] * a.elements[1][1]    * a.elements[2][2] * a.elements[3][3];
	return value;
}

inline Matrix44 rotMat4(Vec3 v, double angle){
	Matrix44 m(0.0);
	double ca = cos(angle);
	double sa = sin(angle);
	double one_ca = 1.0f - ca;

	m.elements[0][0] = (v.a * v.a) * one_ca + ca;
	m.elements[0][1] = (v.a * v.b) * one_ca - (v.c * sa);
	m.elements[0][2] = (v.a * v.c) * one_ca + (v.b * sa);

	m.elements[1][0] = (v.b * v.a) * one_ca + (v.c * sa);
	m.elements[1][1] = (v.b * v.b) * one_ca + ca;
	m.elements[1][2] = (v.b * v.c) * one_ca - (v.a * sa);

	m.elements[2][0] = (v.c * v.a) * one_ca - (v.b * sa);
	m.elements[2][1] = (v.c * v.b) * one_ca + (v.a * sa);
	m.elements[2][2] = (v.c * v.c) * one_ca + ca;

	m.elements[3][3] = 1.0;

	return m;

}

#endif
