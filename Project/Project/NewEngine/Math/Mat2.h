#ifndef MAT2_H
#define MAT2_H

#include "Vector2.h"

namespace Engine
{
	namespace Math
	{
		template<typename Scalar>
		class Mat2
		{
		public:
			Mat2() { }

			explicit Mat2(Scalar s)
				: m_col{ s, 0, 0, s } { }

			explicit Mat2(Scalar s00, Scalar s10, Scalar s01, Scalar s11)
				: m_col{ s00, s10, s01, s11 } { }

			explicit Mat2(const Vector2<Scalar>& col1,
				const Vector2<Scalar>& col2)
				: m_col{ col1, col2 } { }

			Mat2(const Mat2<Scalar>& m)
				: m_col{ m.m_col[0], m.m_col[1] } { }

			Mat2<Scalar>& operator=(const Mat2<Scalar>& m)
			{
				m_col[0] = m.m_col[0];
				m_col[1] = m.m_col[1];
				return *this;
			}

			Mat2<Scalar> operator-()
			{
				return Mat2<Scalar>(-m_col[0], -m_col[1]);
			}

			Mat2<Scalar>& operator+=(const Mat2<Scalar>& m)
			{
				m_col[0] += m.m_col[0];
				m_col[1] += m.m_col[1];
				return *this;
			}

			Mat2<Scalar>& operator-=(const Mat2<Scalar>& m)
			{
				m_col[0] -= m.m_col[0];
				m_col[1] -= m.m_col[1];
				return *this;
			}

			Mat2<Scalar>& operator*=(const Mat2<Scalar>& t)
			{
				const Scalar s00 = m_col[0][0] * t.m_col[0][0] +
					m_col[1][0] * t.m_col[0][1];
				const Scalar s10 = m_col[0][1] * t.m_col[0][0] +
					m_col[1][1] * t.m_col[0][1];

				const Scalar s01 = m_col[0][0] * t.m_col[1][0] +
					m_col[1][0] * t.m_col[1][1];
				const Scalar s11 = m_col[0][1] * t.m_col[1][0] +
					m_col[1][1] * t.m_col[1][1];

				m_col[0] = { s00, s10 };
				m_col[1] = { s01, s11 };
				return *this;
			}

			Mat2<Scalar>& operator/=(const Mat2<Scalar>& m)
			{
				m_col[0] /= m.m_col[0];
				m_col[1] /= m.m_col[1];
				return *this;
			}

			Mat2<Scalar>& operator+=(Scalar s)
			{
				m_col[0] += s;
				m_col[1] += s;
				return *this;
			}

			Mat2<Scalar>& operator-=(Scalar s)
			{
				m_col[0] -= s;
				m_col[1] -= s;
				return *this;
			}

			Mat2<Scalar>& operator*=(Scalar s)
			{
				m_col[0] *= s;
				m_col[1] *= s;
				return *this;
			}

			Mat2<Scalar>& operator/=(Scalar s)
			{
				m_col[0] /= s;
				m_col[1] /= s;
				return *this;
			}

			Mat2<Scalar>& operator++()
			{
				++m_col[0]; ++m_col[1];
				return *this;
			}

			Mat2<Scalar>& operator--()
			{
				--m_col[0]; --m_col[1];
				return *this;
			}

			Mat2<Scalar>& operator++(int)
			{
				Mat2<Scalar> r(*this);
				++m_col[0]; ++m_col[1];
				return r;
			}

			Mat2<Scalar>& operator--(int)
			{
				Mat2<Scalar> r(*this);
				--m_col[0]; --m_col[1];
				return r;
			}

			Vector2<Scalar>& operator[](unsigned i)
			{
				CG_DEBUG_ASSERT(i >= 0 && i <= 1);
				return m_col[i];
			}

			Vector2<Scalar> operator[](unsigned i) const
			{
				CG_DEBUG_ASSERT(i >= 0 && i <= 1);
				return m_col[i];
			}

			Scalar* data() { return m_col; }
			const Scalar* data() const { return m_col; }

			Vector2<Scalar> m_col[2];
		};

		template<typename Scalar>
		inline Mat2<Scalar> operator*(const Mat2<Scalar>& m, Scalar s)
		{
			return Mat2<Scalar>(m.m_col[0] * s, m.m_col[1] * s);
		}

		template<typename Scalar>
		inline Mat2<Scalar> operator*(Scalar s, const Mat2<Scalar>& m)
		{
			return Mat2<Scalar>(s * m.m_col[0], s * m.m_col[1]);
		}

		template<typename Scalar>
		inline Mat2<Scalar> operator+(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			return Mat2<Scalar>(a.m_col[0] + b.m_col[0], a.m_col[1] + b.m_col[1]);
		}

		template<typename Scalar>
		inline Mat2<Scalar> operator-(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			return Mat2<Scalar>(a.m_col[0] - b.m_col[0], a.m_col[1] - b.m_col[1]);
		}

		template<typename Scalar>
		inline Mat2<Scalar> operator*(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			const Scalar s00 = a.m_col[0][0] * b.m_col[0][0] +
				a.m_col[1][0] * b.m_col[0][1];
			const Scalar s10 = a.m_col[0][1] * b.m_col[0][0] +
				a.m_col[1][1] * b.m_col[0][1];

			const Scalar s01 = a.m_col[0][0] * b.m_col[1][0] +
				a.m_col[1][0] * b.m_col[1][1];
			const Scalar s11 = a.m_col[0][1] * b.m_col[1][0] +
				a.m_col[1][1] * b.m_col[1][1];

			return Mat2<Scalar>(s00, s10,
				s01, s11);
		}

		template<typename Scalar>
		inline Mat2<Scalar> operator/(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			return Mat2<Scalar>(a.m_col[0] / b.m_col[0], a.m_col[1] / b.m_col[1]);
		}

		template<typename Scalar>
		inline bool operator==(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			return a.m_col[0] == b.m_col[0] &&
				a.m_col[1] == b.m_col[1];
		}

		template<typename Scalar>
		inline bool operator!=(const Mat2<Scalar>& a, const Mat2<Scalar>& b)
		{
			return a.m_col[0] != b.m_col[0] ||
				a.m_col[1] != b.m_col[1];
		}

		template<typename Scalar>
		inline Mat2<Scalar> Transpose(const Mat2<Scalar>& m)
		{
			return Mat2<Scalar>(m.m_col[0][0], m.m_col[1][0],
				m.m_col[0][1], m.m_col[1][1]);
		}

		template<typename Scalar>
		inline Scalar Determinant(const Mat2<Scalar>& m)
		{
			return m.m_col[0][0] * m.m_col[1][1] - m.m_col[1][0] * m.m_col[0][1];
		}

		template<typename Scalar>
		inline Mat2<Scalar> Inverse(const Mat2<Scalar>& m)
		{
			Mat2<Scalar> r;
			const Scalar oneOverDeterminant = static_cast<Scalar>(1) /
				m.m_col[0][0] * m.m_col[1][1] - m.m_col[1][0] * m.m_col[0][1];

			r.m_col[0][0] = m.m_col[1][1] * oneOverDeterminant;
			r.m_col[0][1] = -m.m_col[0][1] * oneOverDeterminant;
			r.m_col[1][0] = -m.m_col[1][0] * oneOverDeterminant;
			r.m_col[1][1] = m.m_col[0][0] * oneOverDeterminant;

			return r;
		}
	}
}

#endif