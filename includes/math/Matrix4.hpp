#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include <vector>
#include <exception>
#include <cmath>

#include "Types.hpp"
#include "Math.hpp"

class Matrix4
{
	public:
		class OutOfBound : public std::exception
		{
			public:
				const char* what() const throw() {return "Error: Out of bound";};
		};
		class WrongArraySize : public std::exception
		{
			public:
				const char* what() const throw() {return "Error: Array can't go into matrix";};
		};

		Matrix4(float f)
		{
			data.reserve(16);
			for (size_t i = 0; i < 16; i++)
				data.push_back(f);
		};

		Matrix4(const std::vector<float> v)
		{
			if (v.size() != 16)
				throw WrongArraySize();
			data.reserve(16);
			for (size_t i = 0; i < v.size(); i++)
				data.push_back(static_cast<float>(v[i]));
		};

		~Matrix4() {};

		static Matrix4 identity()
		{
			std::vector<float> m(16, 0.0f);
			m[0] = m[5] = m[10] = m[15] = 1.0f;
			return (Matrix4(m));
		}

		Matrix4 mul_mat(const Matrix4& mat) const
		{
			Matrix4 result(0.0f);
			for (size_t row = 0; row < 4; row++)
			{
				for (size_t col = 0; col < 4; col++)
				{
					for (size_t k = 0; k < 4; k++)
						result(col, row) = fma( (*this)(k, row), mat(col, k), result(col, row));
				}
			}
			return result;
		}

		static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			Vector3 f = math::normalize(center - eye);
			Vector3 s = math::normalize(math::cross(f, up));
			Vector3 u = math::cross(s, f);

			Matrix4 mat(0.f);

			mat(0, 0) = s.x;
			mat(0, 1) = u.x;
			mat(0, 2) = -f.x;

			mat(1, 0) = s.y;
			mat(1, 1) = u.y;
			mat(1, 2) = -f.y;

			mat(2, 0) = s.z;
			mat(2, 1) = u.z;
			mat(2, 2) = -f.z;

			mat(3, 0) = -math::dot(s, eye);
			mat(3, 1) = -math::dot(u, eye);
			mat(3, 2) = math::dot(f, eye);
			mat(3, 3) = 1.0f;
			
			return mat;
		}

		static Matrix4 perspective(float fov, float aspect, float near, float far)
		{
			float tanHalfFov = std::tan(fov / 2.0f);

			Matrix4 mat(0.0f);
			
			mat(0, 0) = 1.0f / (aspect * tanHalfFov);
			mat(1, 1) = 1.0f / tanHalfFov;
			mat(2, 2) = - (far + near) / (far - near);
			mat(2, 3) = -1.0f;
			mat(3, 2) = - (2.0f * far * near) / (far - near);
			return mat;
		}

		float& operator()(size_t c, size_t r)
		{
			size_t idx = c * 4 + r;
			if (idx >= data.size())
				throw OutOfBound();
			return (data[idx]);
		}

		const float& operator()(size_t c, size_t r) const
		{
			size_t idx = c * 4 + r;
			if (idx >= data.size())
				throw OutOfBound();
			return (data[idx]);
		}

		const float* dataPtr() const
		{
			return data.data();
		}

		static Matrix4 rotateX(float angle)
		{
			float cos = std::cos(angle);
    		float sin = std::sin(angle);
			Matrix4 mat = identity();
			mat(1, 1) = cos;  mat(2, 1) = sin;
			mat(1, 2) = -sin; mat(2, 2) = cos;
			return (mat);
		}

		static Matrix4 rotateY(float angle)
		{
			float cos = std::cos(angle);
    		float sin = std::sin(angle);
			Matrix4 mat = identity();
			mat(0, 0) = cos; mat(2, 0) = -sin;
			mat(0, 2) = sin; mat(2, 2) = cos;
			return (mat);
		}

		static Matrix4 rotateZ(float angle)
		{
			float cos = std::cos(angle);
    		float sin = std::sin(angle);
			Matrix4 mat = identity();
			mat(0, 0) = cos;  mat(1, 0) = sin;
			mat(0, 1) = -sin; mat(1, 1) = cos;
			return (mat);
		}

	private:
		std::vector<float> data;
};

using mat4 = Matrix4;

#endif