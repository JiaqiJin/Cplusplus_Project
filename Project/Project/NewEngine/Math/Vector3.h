#ifndef VECTOR_3_H
#define VECTOR_3_H

#include "MathConfig.h"

namespace Engine
{
	namespace Math
	{
		template <typename Scalar>
		class Vector3
		{
		public:
			Vector3() { }
			explicit Vector3(Scalar _x, Scalar _y, Scalar _z) : m_value{ _x, _y,_z } { }
			explicit Vector3(Scalar fill) : m_value{ fill, fill, fill } { }
			Vector3(const Vector3<Scalar>& v) : m_value{ v.m_value[0], v.m_value[1], v.m_value[2] } { }

			Vector3<Scalar>& operator=(const Vector3<Scalar>& v)
			{
				m_value[0] = v.m_value[0];
				m_value[1] = v.m_value[1];
				m_value[2] = v.m_value[2];
				return *this;
			}

			Vector3<Scalar> operator-()
			{
				return Vector3<Scalar>(-m_value[0], -m_value[1], -m_value[2]);
			}

			Vector3<Scalar>& operator +=(const Vector3<Scalar>& v)
			{
				m_value[0] += v.m_value[0];
				m_value[1] += v.m_value[1];
				m_value[2] += v.m_value[2];
				return *this;
			}

			Vector3<Scalar>& operator -=(const Vector3<Scalar>& v)
			{
				m_value[0] -= v.m_value[0];
				m_value[1] -= v.m_value[1];
				m_value[2] -= v.m_value[2];
				return *this;
			}

			Vector3<Scalar>& operator *=(const Vector3<Scalar>& v)
			{
				m_value[0] *= v.m_value[0];
				m_value[1] *= v.m_value[1];
				m_value[2] *= v.m_value[2];
				return *this;
			}

			Vector3<Scalar>& operator /=(const Vector3<Scalar>& v)
			{
				m_value[0] /= v.m_value[0];
				m_value[1] /= v.m_value[1];
				m_value[2] /= v.m_value[2];
				return *this;
			}

			Vector3<Scalar>& operator +=(Scalar s)
			{
				m_value[0] += s;
				m_value[1] += s;
				m_value[2] += s;
				return *this;
			}

			Vector3<Scalar>& operator -=(Scalar s)
			{
				m_value[0] -= s;
				m_value[1] -= s;
				m_value[2] -= s;
				return *this;
			}

			Vector3<Scalar>& operator *=(Scalar s)
			{
				m_value[0] *= s;
				m_value[1] *= s;
				m_value[2] *= s;
				return *this;
			}

			Vector3<Scalar>& operator /=(Scalar s)
			{
				m_value[0] /= s;
				m_value[1] /= s;
				m_value[2] /= s;
				return *this;
			}

			Vector3<Scalar>& operator++()
			{
				++m_value[0]; ++m_value[1]; ++m_value[2];
				return *this;
			}

			Vector3<Scalar>& operator--()
			{
				--m_value[0]; --m_value[1]; --m_value[2];
				return *this;
			}

			Vector3<Scalar> operator++(int) // postfix
			{
				Vector3<Scalar> r(*this);
				++m_value[0]; ++m_value[1]; ++m_value[2];
				return *this;
			}

			Vector3<Scalar> operator--(int) // postfix
			{
				Vector3<Scalar> r(*this);
				--m_value[0]; --m_value[1]; --m_value[2];
				return *this;
			}

			template<typename IndexType>
			Scalar operator[](IndexType i) const
			{
				return m_value[i];
			}

			template<typename IndexType>
			Scalar& operator[](IndexType i)
			{
				return m_value[i];
			}

			Scalar* data() { return m_value; }
			const Scalar* data() const { return m_value; }

			Scalar m_value[3];
		};

		template <typename Scalar>
		inline Vector3<Scalar> operator*(const Vector3<Scalar>& v, const Scalar& scalar)
		{
			return Vector3<Scalar>(v.m_value[0] * scalar, v.m_value[1] * scalar, v.m_value[2] * scalar);
		}

		template <typename Scalar>
		inline Vector3<Scalar> operator*(const Scalar& scalar, const Vector3<Scalar>& v)
		{
			return Vector3<Scalar>(scalar * v.m_value[0], scalar * v.m_value[1], scalar * v.m_value[2]);
		}

		template <typename Scalar>
		inline Vector3<Scalar> operator+(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] + b.m_value[0], a.m_value[1] + b.m_value[1], a.m_value[2] + b.m_value[2]);
		}

		template <typename Scalar>
		inline Vector3<Scalar> operator-(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] - b.m_value[0], a.m_value[1] - b.m_value[1], a.m_value[2] - b.m_value[2]);
		}

		template <typename Scalar>
		inline Vector3<Scalar> operator*(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] * b.m_value[0], a.m_value[1] * b.m_value[1], a.m_value[2] * b.m_value[2]);
		}

		template <typename Scalar>
		inline Vector3<Scalar> operator/(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] / b.m_value[0], a.m_value[1] / b.m_value[1], a.m_value[2] / b.m_value[2]);
		}

		template <typename Scalar>
		inline bool operator==(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return a.m_value[0] == b.m_value[0] &&
				a.m_value[1] == b.m_value[1] &&
				a.m_value[2] == b.m_value[2];
		}

		template <typename Scalar>
		inline bool operator!=(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return a.m_value[0] != b.m_value[0] ||
				a.m_value[1] != b.m_value[1] ||
				a.m_value[2] != b.m_value[2];
		}

		template <typename Scalar>
		inline Scalar Dot(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return a.m_value[0] * b.m_value[0] + a.m_value[1] * b.m_value[1] + a.m_value[2] * b.m_value[2];
		}

		template <typename Scalar>
		inline Vector3<Scalar> Cross(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[1] * b.m_value[2] - a.m_value[2] * b.m_value[1],
				a.m_value[2] * b.m_value[0] - a.m_value[0] * b.m_value[2],
				a.m_value[0] * b.m_value[1] - a.m_value[1] * b.m_value[0]);
		}

		template <typename Scalar>
		inline Scalar Length(const Vector3<Scalar>& v)
		{
			return Scalar_traits<Scalar>::sqrt(v.m_value[0] * v.m_value[0] + v.m_value[1] * v.m_value[1] + v.m_value[2] * v.m_value[2]);
		}

		template<typename Scalar>
		inline Scalar Distance(const Vector3<Scalar>& a, const Vector3<Scalar>& b)
		{
			return Length(a - b);
		}

		template <typename Scalar>
		inline Vector3<Scalar> Normalize(const Vector3<Scalar>& v)
		{
			Scalar length = Length(v);
			return v * (static_cast<Scalar>(1.0) / length);
		}
	}
}

#endif