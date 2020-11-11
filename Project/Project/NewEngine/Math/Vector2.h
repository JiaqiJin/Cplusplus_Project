#ifndef VECTOR_2_H
#define VECTOR_2_H

#include "MathConfig.h"

namespace Engine
{
	namespace Math
	{
		template<typename Scalar>
		class Vector2
		{
		public:
			Vector2() {}
			explicit Vector2(Scalar _x, Scalar _y) : m_value{ _x, _y } { }
			explicit Vector2(Scalar fill) : m_value{ fill, fill } { }
			Vector2<Scalar>& operator=(const Vector2<Scalar>& v)
			{
				m_value[0] = v.m_value[0];
				m_value[1] = v.m_value[1];
				return *this;
			}

			Vector2<Scalar> operator-()
			{
				return Vector2<Scalar>(-m_value[0], -m_value[1]);
			}

			Vector2<Scalar>& operator +=(const Vector2<Scalar>& v)
			{
				m_value[0] += v.m_value[0];
				m_value[1] += v.m_value[1];
				return *this;
			}

			Vector2<Scalar>& operator -=(const Vector2<Scalar>& v)
			{
				m_value[0] -= v.m_value[0];
				m_value[1] -= v.m_value[1];
				return *this;
			}

			Vector2<Scalar>& operator *=(const Vector2<Scalar>& v)
			{
				m_value[0] *= v.m_value[0];
				m_value[1] *= v.m_value[1];
				return *this;
			}

			Vector2<Scalar>& operator /=(const Vector2<Scalar>& v)
			{
				m_value[0] /= v.m_value[0];
				m_value[1] /= v.m_value[1];
				return *this;
			}

			Vector2<Scalar>& operator +=(Scalar s)
			{
				m_value[0] += s;
				m_value[1] += s;
				return *this;
			}

			Vector2<Scalar>& operator -=(Scalar s)
			{
				m_value[0] -= s;
				m_value[1] -= s;
				return *this;
			}

			Vector2<Scalar>& operator *=(Scalar s)
			{
				m_value[0] *= s;
				m_value[1] *= s;
				return *this;
			}

			Vector2<Scalar>& operator /=(Scalar s)
			{
				m_value[0] /= s;
				m_value[1] /= s;
				return *this;
			}

			Vector2<Scalar>& operator++()
			{
				++m_value[0]; ++m_value[1];
				return *this;
			}

			Vector2<Scalar>& operator--()
			{
				--m_value[0]; --m_value[1];
				return *this;
			}

			Vector2<Scalar> operator++(int) // postfix
			{
				Vector2<Scalar> r(*this);
				++m_value[0]; ++m_value[1];
				return *this;
			}

			Vector2<Scalar> operator--(int) // postfix
			{
				Vector2<Scalar> r(*this);
				--m_value[0]; --m_value[1];
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

		private:
			Scalar m_value[2];
		};

		//-------------Functions ---------------------------
		template <typename Scalar>
		inline Vector2<Scalar> operator*(const Vector2<Scalar>& v, const Scalar& scalar)
		{
			return Vector2<Scalar>(v.m_value[0] * scalar, v.m_value[1] * scalar);
		}

		template <typename Scalar>
		inline Vector2<Scalar> operator*(const Scalar& scalar, const Vector2<Scalar>& v)
		{
			return Vector2<Scalar>(scalar * v.m_value[0], scalar * v.m_value[1]);
		}

		template <typename Scalar>
		inline Vector2<Scalar> operator+(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return Vector2<Scalar>(a.m_value[0] + b.m_value[0], a.m_value[1] + b.m_value[1]);
		}

		template <typename Scalar>
		inline Vector2<Scalar> operator-(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return Vector2<Scalar>(a.m_value[0] - b.m_value[0], a.m_value[1] - b.m_value[1]);
		}

		template <typename Scalar>
		inline Vector2<Scalar> operator*(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return Vector2<Scalar>(a.m_value[0] * b.m_value[0], a.m_value[1] * b.m_value[1]);
		}

		template <typename Scalar>
		inline Vector2<Scalar> operator/(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return Vector2<Scalar>(a.m_value[0] / b.m_value[0], a.m_value[1] / b.m_value[1]);
		}

		template <typename Scalar>
		inline bool operator==(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return a.m_value[0] == b.m_value[0] &&
				a.m_value[1] == b.m_value[1];
		}

		template <typename Scalar>
		inline bool operator!=(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return a.m_value[0] != b.m_value[0] ||
				a.m_value[1] != b.m_value[1];
		}

		template <typename Scalar>
		inline Scalar Dot(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return a.m_value[0] * b.m_value[0] + a.m_value[1] * b.m_value[1];
		}

		template <typename Scalar>
		inline Scalar Cross(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return a.x * b.y - a.y * b.x;
		}

		template <typename Scalar>
		inline Vector2<Scalar> Cross(const Vector2<Scalar>& v, Scalar s)
		{
			return Vector2<Scalar>(v.y, -s * v.x);
		}

		template <typename Scalar>
		inline Vector2<Scalar> Cross(Scalar s, const Vector2<Scalar> v)
		{
			return Vector2<Scalar>(-s * v.y, v.x);
		}

		template <typename Scalar>
		inline Scalar Length(const Vector2<Scalar>& v)
		{
			return Scalar_traits<Scalar>::sqrt(v.m_value[0] * v.m_value[0] + v.m_value[1] * v.m_value[1]);
		}

		template<typename Scalar>
		inline Scalar Distance(const Vector2<Scalar>& a, const Vector2<Scalar>& b)
		{
			return Length(a - b);
		}

		template <typename Scalar>
		inline Vector2<Scalar> Normalize(const Vector2<Scalar>& v)
		{
			Scalar length = Length(v);
			return v * (static_cast<Scalar>(1.0) / length);
		}

	}
}

#endif
