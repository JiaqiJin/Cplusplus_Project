#ifndef MAT3_H
#define MAT3_H

#include "Vector3.h"

namespace Engine
{
	namespace Math
	{
		template<typename Scalar>
		class Mat3
		{
		public:
			Mat3() { }

			explicit Mat3(Scalar s)
				: m_col{ s, 0, 0,
				0, s, 0,
				0, 0, s } { }

			explicit Mat3(Scalar* s)
			{
				memcpy(m_col->data(), s, sizeof(Scalar) * 9);
			}

			explicit Mat3(const Vector3<Scalar>& col1,
				const Vector3<Scalar>& col2,
				const Vector3<Scalar>& col3)
				: m_col{ col1, col2, col3 } { }

			Mat3(const Mat3<Scalar>& m)
				: m_col{ m.m_col[0], m.m_col[1], m.m_col[2] } { }

			Mat3<Scalar>& operator=(const Mat3<Scalar>& m)
			{
				m_col[0] = m.m_col[0];
				m_col[1] = m.m_col[1];
				m_col[2] = m.m_col[2];
				return *this;
			}

			Mat3<Scalar> operator-()
			{
				return Mat3(-m_col[0], -m_col[1], -m_col[2]);
			}

			Mat3<Scalar>& operator+=(const Mat3<Scalar>& m)
			{
				m_col[0] += m.m_col[0];
				m_col[1] += m.m_col[1];
				m_col[2] += m.m_col[2];
				return *this;
			}

			Mat3<Scalar>& operator-=(const Mat3<Scalar>& m)
			{
				m_col[0] -= m.m_col[0];
				m_col[1] -= m.m_col[1];
				m_col[2] -= m.m_col[2];
				return *this;
			}

			Mat3<Scalar>& operator*=(const Mat3<Scalar>& t)
			{
				const Scalar s00 = m_col[0][0] * t.m_col[0][0] +
					m_col[1][0] * t.m_col[0][1] +
					m_col[2][0] * t.m_col[0][2];
				const Scalar s10 = m_col[0][1] * t.m_col[0][0] +
					m_col[1][1] * t.m_col[0][1] +
					m_col[2][1] * t.m_col[0][2];
				const Scalar s20 = m_col[0][2] * t.m_col[0][0] +
					m_col[1][2] * t.m_col[0][1] +
					m_col[2][2] * t.m_col[0][2];

				const Scalar s01 = m_col[0][0] * t.m_col[1][0] +
					m_col[1][0] * t.m_col[1][1] +
					m_col[2][0] * t.m_col[1][2];
				const Scalar s11 = m_col[0][1] * t.m_col[1][0] +
					m_col[1][1] * t.m_col[1][1] +
					m_col[2][1] * t.m_col[1][2];
				const Scalar s21 = m_col[0][2] * t.m_col[1][0] +
					m_col[1][2] * t.m_col[1][1] +
					m_col[2][2] * t.m_col[1][2];

				const Scalar s02 = m_col[0][0] * t.m_col[2][0] +
					m_col[1][0] * t.m_col[2][1] +
					m_col[2][0] * t.m_col[2][2];
				const Scalar s12 = m_col[0][1] * t.m_col[2][0] +
					m_col[1][1] * t.m_col[2][1] +
					m_col[2][1] * t.m_col[2][2];
				const Scalar s22 = m_col[0][2] * t.m_col[2][0] +
					m_col[1][2] * t.m_col[2][1] +
					m_col[2][2] * t.m_col[2][2];

				m_col[0] = { s00, s10, s20 };
				m_col[1] = { s01, s11, s21 };
				m_col[2] = { s02, s12, s22 };
				return *this;
			}

			Mat3<Scalar>& operator/=(const Mat3<Scalar>& m)
			{
				m_col[0] /= m.m_col[0];
				m_col[1] /= m.m_col[1];
				m_col[2] /= m.m_col[2];
				return *this;
			}

			Mat3<Scalar>& operator+=(Scalar s)
			{
				m_col[0] += s;
				m_col[1] += s;
				m_col[2] += s;
				return *this;
			}

			Mat3<Scalar>& operator-=(Scalar s)
			{
				m_col[0] -= s;
				m_col[1] -= s;
				m_col[2] -= s;
				return *this;
			}

			Mat3<Scalar>& operator*=(Scalar s)
			{
				m_col[0] *= s;
				m_col[1] *= s;
				m_col[2] *= s;
				return *this;
			}

			Mat3<Scalar>& operator/=(Scalar s)
			{
				m_col[0] /= s;
				m_col[1] /= s;
				m_col[2] /= s;
				return *this;
			}

			Mat3<Scalar>& operator++()
			{
				++m_col[0]; ++m_col[1]; ++m_col[2];
				return *this;
			}

			Mat3<Scalar>& operator--()
			{
				--m_col[0]; --m_col[1]; --m_col[2];
				return *this;
			}

			Mat3<Scalar>& operator++(int)
			{
				Mat3<Scalar> r(*this);
				++m_col[0]; ++m_col[1]; ++m_col[2];
				return r;
			}

			Mat3<Scalar>& operator--(int)
			{
				Mat3<Scalar> r(*this);
				--m_col[0]; --m_col[1]; --m_col[2];
				return r;
			}

			Vector3<Scalar>& operator[](unsigned i)
			{
				CG_DEBUG_ASSERT(i >= 0 && i <= 2);
				return m_col[i];
			}

			Vector3<Scalar> operator[](unsigned i) const
			{
				CG_DEBUG_ASSERT(i >= 0 && i <= 2);
				return m_col[i];
			}

			Scalar* data() { return m_col; }
			const Scalar* data() const { return m_col; }

			Vector3<Scalar> m_col[3];
		};

		template<typename Scalar>
		inline Mat3<Scalar> operator*(const Mat3<Scalar>& m, Scalar s)
		{
			return Mat3<Scalar>(m.m_col[0] * s, m.m_col[1] * s, m.m_col[2] * s);
		}

		template<typename Scalar>
		inline Mat3<Scalar> operator*(Scalar s, const Mat3<Scalar>& m)
		{
			return Mat3<Scalar>(s * m.m_col[0], s * m.m_col[1], s * m.m_col[2]);
		}

		template<typename Scalar>
		inline Mat3<Scalar> operator+(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			return Mat3<Scalar>(a.m_col[0] + b.m_col[0], a.m_col[1] + b.m_col[1], a.m_col[2] + b.m_col[2]);
		}

		template<typename Scalar>
		inline Mat3<Scalar> operator-(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			return Mat3<Scalar>(a.m_col[0] - b.m_col[0], a.m_col[1] - b.m_col[1], a.m_col[2] - b.m_col[2]);
		}

		template<typename Scalar>
		inline Mat3<Scalar> operator*(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			Mat3<Scalar> r;
			r.m_col[0][0] = a.m_col[0][0] * b.m_col[0][0] +
				a.m_col[1][0] * b.m_col[0][1] +
				a.m_col[2][0] * b.m_col[0][2];
			r.m_col[0][1] = a.m_col[0][1] * b.m_col[0][0] +
				a.m_col[1][1] * b.m_col[0][1] +
				a.m_col[2][1] * b.m_col[0][2];
			r.m_col[0][2] = a.m_col[0][2] * b.m_col[0][0] +
				a.m_col[1][2] * b.m_col[0][1] +
				a.m_col[2][2] * b.m_col[0][2];

			r.m_col[1][0] = a.m_col[0][0] * b.m_col[1][0] +
				a.m_col[1][0] * b.m_col[1][1] +
				a.m_col[2][0] * b.m_col[1][2];
			r.m_col[1][1] = a.m_col[0][1] * b.m_col[1][0] +
				a.m_col[1][1] * b.m_col[1][1] +
				a.m_col[2][1] * b.m_col[1][2];
			r.m_col[1][2] = a.m_col[0][2] * b.m_col[1][0] +
				a.m_col[1][2] * b.m_col[1][1] +
				a.m_col[2][2] * b.m_col[1][2];

			r.m_col[2][0] = a.m_col[0][0] * b.m_col[2][0] +
				a.m_col[1][0] * b.m_col[2][1] +
				a.m_col[2][0] * b.m_col[2][2];
			r.m_col[2][1] = a.m_col[0][1] * b.m_col[2][0] +
				a.m_col[1][1] * b.m_col[2][1] +
				a.m_col[2][1] * b.m_col[2][2];
			r.m_col[2][2] = a.m_col[0][2] * b.m_col[2][0] +
				a.m_col[1][2] * b.m_col[2][1] +
				a.m_col[2][2] * b.m_col[2][2];

			return r;
		}

		template<typename Scalar>
		inline Mat3<Scalar> operator/(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			return Mat3<Scalar>(a.m_col[0] / b.m_col[0], a.m_col[1] / b.m_col[1], a.m_col[2] / b.m_col[2]);
		}

		template<typename Scalar>
		inline bool operator==(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			return a.m_col[0] == b.m_col[0] &&
				a.m_col[1] == b.m_col[1] &&
				a.m_col[2] == b.m_col[2];
		}

		template<typename Scalar>
		inline bool operator!=(const Mat3<Scalar>& a, const Mat3<Scalar>& b)
		{
			return a.m_col[0] != b.m_col[0] ||
				a.m_col[1] != b.m_col[1] ||
				a.m_col[2] != b.m_col[2];
		}

		template<typename Scalar>
		inline Mat3<Scalar> Transpose(const Mat3<Scalar>& m)
		{
			Mat3<Scalar> r;
			r.m_col[0][0] = m.m_col[0][0];
			r.m_col[0][1] = m.m_col[1][0];
			r.m_col[0][2] = m.m_col[2][0];
			r.m_col[1][0] = m.m_col[0][1];
			r.m_col[1][1] = m.m_col[1][1];
			r.m_col[1][2] = m.m_col[2][1];
			r.m_col[2][0] = m.m_col[0][2];
			r.m_col[2][1] = m.m_col[1][2];
			r.m_col[2][2] = m.m_col[2][2];
			return r;
		}

		template<typename Scalar>
		inline Scalar Determinant(const Mat3<Scalar>& m)
		{
			// Laplace's formula. Calculated along the first row
			return m.m_col[0][0] * (m.m_col[1][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[1][2])
				- m.m_col[1][0] * (m.m_col[0][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[0][2])
				+ m.m_col[2][0] * (m.m_col[0][1] * m.m_col[1][2] - m.m_col[1][1] * m.m_col[0][2]);
		}

		template<typename Scalar>
		inline Mat3<Scalar> Inverse(const Mat3<Scalar>& m)
		{
			Mat3<Scalar> r;
			const Scalar oneOverDeterminant = static_cast<Scalar>(1) /
				m.m_col[0][0] * (m.m_col[1][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[1][2])
				- m.m_col[1][0] * (m.m_col[0][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[0][2])
				+ m.m_col[2][0] * (m.m_col[0][1] * m.m_col[1][2] - m.m_col[1][1] * m.m_col[0][2]);

			r.m_col[0][0] = +(m.m_col[1][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[1][2]) * oneOverDeterminant;
			r.m_col[0][1] = -(m.m_col[0][1] * m.m_col[2][2] - m.m_col[2][1] * m.m_col[0][2]) * oneOverDeterminant;
			r.m_col[0][2] = +(m.m_col[0][1] * m.m_col[1][2] - m.m_col[1][1] * m.m_col[0][2]) * oneOverDeterminant;
			r.m_col[1][0] = -(m.m_col[1][0] * m.m_col[2][2] - m.m_col[2][0] * m.m_col[1][2]) * oneOverDeterminant;
			r.m_col[1][1] = +(m.m_col[0][0] * m.m_col[2][2] - m.m_col[2][0] * m.m_col[0][2]) * oneOverDeterminant;
			r.m_col[1][2] = -(m.m_col[0][0] * m.m_col[1][2] - m.m_col[1][0] * m.m_col[0][2]) * oneOverDeterminant;
			r.m_col[2][0] = +(m.m_col[1][0] * m.m_col[2][1] - m.m_col[2][0] * m.m_col[1][1]) * oneOverDeterminant;
			r.m_col[2][1] = -(m.m_col[0][0] * m.m_col[2][1] - m.m_col[2][0] * m.m_col[0][1]) * oneOverDeterminant;
			r.m_col[2][2] = +(m.m_col[0][0] * m.m_col[1][1] - m.m_col[1][0] * m.m_col[0][2]) * oneOverDeterminant;

			return r;
		}
	}
}

#endif