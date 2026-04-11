#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#include "Types.hpp"

namespace math
{
    inline float radians(float degrees)
    {
        return degrees * (M_PI / 180.0f);
    }

    inline float degrees(float radians)
    {
        return radians * (180.0f / M_PI);
    }

	inline float dot(const vec3& a, const vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline vec3 cross(const vec3& a, const vec3& b)
	{
		return vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	inline vec3 normalize(const vec3& v)
	{
		float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		if (len == 0.0f)
			return vec3{0,0,0};
		return vec3(v.x / len, v.y / len, v.z / len);
	}
}


#endif