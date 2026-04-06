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
			std::vector<float> array(16, float {});
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					for (size_t k = 0; k < 4; k++)
					{
						size_t idx = i + j * 4;
						array[idx] = fma( (*this)(i, k), mat(k, j), array[idx]);
					}
				}
			}
			return Matrix4(array);
		}

		static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
		{
			Vector3 f = math::normalize(center - eye);
			Vector3 s = math::normalize(math::cross(f, up));
			Vector3 u = math::cross(s, f);

			std::vector<float> array(16, 0.0f);

			array[0] = s.x; array[4] = s.y; array[8]  = s.z; array[12] = -math::dot(s, eye);
			array[1] = u.x; array[5] = u.y; array[9]  = u.z; array[13] = -math::dot(u, eye);
			array[2] = -f.x; array[6] = -f.y; array[10] = -f.z; array[14] = math::dot(f, eye);
			array[3] = 0.0f; array[7] = 0.0f; array[11] = 0.0f; array[15] = 1.0f;

			return Matrix4(array);
		}

		static Matrix4 perspective(float fov, float aspect, float near, float far)
		{
			float tanHalfFov = std::tan(fov / 2.0f);

			std::vector<float> array(16, 0.0f);
			array[0] = 1.0f / (aspect * tanHalfFov);
			array[5] = 1.0f / tanHalfFov;
			array[10] = - (far + near) / (far - near);
			array[11] = -1.0f;
			array[14] = - (2.0f * far * near) / (far - near);
			return Matrix4(array);
		}

		const float& operator()(size_t c, size_t r) const
		{
			size_t idx = c + r * 4;
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
			std::vector mat(16, 0.0f);
			mat[0]  = 1;
			mat[5]  = cos;  mat[9]  = sin;
			mat[6]  = -sin;  mat[10] = cos;
			mat[15] = 1;
			return (Matrix4(mat));
		}

		static Matrix4 rotateY(float angle)
		{
			float cos = std::cos(angle);
    		float sin = std::sin(angle);
			std::vector mat(16, 0.0f);
			mat[0]  = cos;  mat[8]  = -sin;
			mat[5]  = 1;
			mat[2]  = sin; mat[10] = cos;
			mat[15] = 1;
			return (Matrix4(mat));
		}

		static Matrix4 rotateZ(float angle)
		{
			float cos = std::cos(angle);
    		float sin = std::sin(angle);
			std::vector mat(16, 0.0f);
			mat[0]  = cos;  mat[4]  = sin;
			mat[1]  = -sin;  mat[5]  = cos;
			mat[10] = 1;
			mat[15] = 1;
			return (Matrix4(mat));
		}

	private:
		std::vector<float> data;
};

using mat4 = Matrix4;

#endif