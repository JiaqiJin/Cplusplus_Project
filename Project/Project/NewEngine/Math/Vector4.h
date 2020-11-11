#ifndef VECTOR_4_H
#define VECTOR_4_H

#include "MathConfig.h"

namespace Engine
{
	namespace Math
	{
		template <typename Scalar>
		class Vector4
		{
		public:
			Vector4() { }
			explicit Vector4(Scalar _x, Scalar _y, Scalar _z, Scalar _w) : m_value{ _x, _y,_z , _w } { }
			explicit Vector4(Scalar fill) : m_value{ fill, fill, fill, fill } { }
			Vector4(const Vector4<Scalar>& v) : m_value{ v.m_value[0], v.m_value[1], v.m_value[2], v.m_value[3] } { }

			Vector4<Scalar>& operator=(const Vector4<Scalar>& v)
			{
				m_value[0] = v.m_value[0];
				m_value[1] = v.m_value[1];
				m_value[2] = v.m_value[2];
				m_value[3] = v.m_value[3];
				return *this;
			}

			Vector4<Scalar> operator-()
			{
				return Vector4<Scalar>(-m_value[0], -m_value[1], -m_value[2], -m_value[3]);
			}

			Vector4<Scalar>& operator +=(const Vector4<Scalar>& v)
			{
				m_value[0] += v.m_value[0];
				m_value[1] += v.m_value[1];
				m_value[2] += v.m_value[2];
				m_value[3] += v.m_value[3];
				return *this;
			}

			Vector4<Scalar>& operator -=(const Vector4<Scalar>& v)
			{
				m_value[0] -= v.m_value[0];
				m_value[1] -= v.m_value[1];
				m_value[2] -= v.m_value[2];
				m_value[3] -= v.m_value[3];
				return *this;
			}

			Vector4<Scalar>& operator *=(const Vector4<Scalar>& v)
			{
				m_value[0] *= v.m_value[0];
				m_value[1] *= v.m_value[1];
				m_value[2] *= v.m_value[2];
				m_value[3] *= v.m_value[3];
				return *this;
			}

			Vector4<Scalar>& operator /=(const Vector4<Scalar>& v)
			{
				m_value[0] /= v.m_value[0];
				m_value[1] /= v.m_value[1];
				m_value[2] /= v.m_value[2];
				m_value[3] /= v.m_value[3];
				return *this;
			}

			Vector4<Scalar>& operator +=(Scalar s)
			{
				m_value[0] += s;
				m_value[1] += s;
				m_value[2] += s;
				m_value[3] += s;
				return *this;
			}

			Vector4<Scalar>& operator -=(Scalar s)
			{
				m_value[0] -= s;
				m_value[1] -= s;
				m_value[2] -= s;
				m_value[3] -= s;
				return *this;
			}

			Vector4<Scalar>& operator *=(Scalar s)
			{
				m_value[0] *= s;
				m_value[1] *= s;
				m_value[2] *= s;
				m_value[3] *= s;
				return *this;
			}

			Vector4<Scalar>& operator /=(Scalar s)
			{
				m_value[0] /= s;
				m_value[1] /= s;
				m_value[2] /= s;
				m_value[3] /= s;
				return *this;
			}

			Vector4<Scalar>& operator++()
			{
				++m_value[0]; ++m_value[1]; ++m_value[2]; ++m_value[3];
				return *this;
			}

			Vector4<Scalar>& operator--()
			{
				--m_value[0]; --m_value[1]; --m_value[2]; --m_value[3];
				return *this;
			}

			Vector4<Scalar> operator++(int) // postfix
			{
				Vector3<Scalar> r(*this);
				++m_value[0]; ++m_value[1]; ++m_value[2]; ++m_value[3];
				return *this;
			}

			Vector4<Scalar> operator--(int) // postfix
			{
				Vector3<Scalar> r(*this);
				--m_value[0]; --m_value[1]; --m_value[2]; --m_value[3];
				return *this;
			}

			template<typename indexType>
			Scalar operator[](indexType i) const
			{
				return m_value[i];
			}

			template<typename indexType>
			Scalar& operator[](indexType i)
			{
				return m_value[i];
			}

			Scalar* data() { return m_value; }
			const Scalar* data() const { return m_value; }

			Scalar m_value[4];
		};

		template <typename Scalar>
		inline Vector4<Scalar> operator*(const Vector4<Scalar>& v, const Scalar& scalar)
		{
			return Vector4<Scalar>(v.m_value[0] * scalar, v.m_value[1] * scalar, v.m_value[2] * scalar, v.m_value[3] * scalar);
		}

		template <typename Scalar>
		inline Vector4<Scalar> operator*(const Scalar& scalar, const Vector4<Scalar>& v)
		{
			return Vector4<Scalar>(scalar * v.m_value[0], scalar * v.m_value[1], scalar * v.m_value[2], scalar * v.m_value[3]);
		}

		template <typename Scalar>
		inline Vector4<Scalar> operator+(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] + b.m_value[0], a.m_value[1] + b.m_value[1], a.m_value[2] + b.m_value[2], a.m_value[3] + b.m_value[3]);
		}

		template <typename Scalar>
		inline Vector4<Scalar> operator-(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] - b.m_value[0], a.m_value[1] - b.m_value[1], a.m_value[2] - b.m_value[2], a.m_value[3] - b.m_value[3]);
		}

		template <typename Scalar>
		inline Vector4<Scalar> operator*(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] * b.m_value[0], a.m_value[1] * b.m_value[1], a.m_value[2] * b.m_value[2], a.m_value[3] * b.m_value[3]);
		}

		template <typename Scalar>
		inline Vector4<Scalar> operator/(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return Vector3<Scalar>(a.m_value[0] / b.m_value[0], a.m_value[1] / b.m_value[1], a.m_value[2] / b.m_value[2], a.m_value[3] / b.m_value[3]);
		}

		template <typename Scalar>
		inline bool operator==(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return a.m_value[0] == b.m_value[0] &&
				a.m_value[1] == b.m_value[1] &&
				a.m_value[2] == b.m_value[2] &&
				a.m_value[3] == b.m_value[3];
		}

		template <typename Scalar>
		inline bool operator!=(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return a.m_value[0] != b.m_value[0] ||
				a.m_value[1] != b.m_value[1] ||
				a.m_value[2] != b.m_value[2] ||
				a.m_value[3] != b.m_value[3];
		}

		template <typename Scalar>
		inline Scalar Dot(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return a.m_value[0] * b.m_value[0] + a.m_value[1] * b.m_value[1] + a.m_value[2] * b.m_value[2] + a.m_value[3] * b.m_value[3];
		}

		template <typename Scalar>
		inline Scalar Length(const Vector4<Scalar>& v)
		{
			return Scalar_traits<Scalar>::sqrt(v.m_value[0] * v.m_value[0] +
				v.m_value[1] * v.m_value[1] +
				v.m_value[2] * v.m_value[2] +
				v.m_value[3] * v.m_value[3]);
		}

		template<typename Scalar>
		inline Scalar Distance(const Vector4<Scalar>& a, const Vector4<Scalar>& b)
		{
			return Length(a - b);
		}

		template <typename Scalar>
		inline Vector4<Scalar> Normalize(const Vector4<Scalar>& v)
		{
			Scalar length = Length(v);
			CG_DEBUG_BREAK(length);
			return v * (static_cast<Scalar>(1.0) / length);
		}
	}
}

#endif