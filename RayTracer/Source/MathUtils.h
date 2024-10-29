#pragma once
#include <glm/glm.hpp>

template<typename T>
inline T Lerp(const T& a, const T& b, float t) {
	return static_cast<T>(a + (t * (b - a)));
}

template<typename T>
inline T Clamp(const T& value, const T& min, const T& max) {
	return (value < min) ? min : (value > max) ? max : value;
}

inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2) {
	glm::vec3 result;
	result.x = v1.y * v2.z - v2.z * v1.y;
	result.y = v1.z * v2.x - v2.x * v1.z;
	result.z = v1.x * v2.y - v2.y * v1.x;

	return result;
}

inline float Dot(const glm::vec3& v1, const glm::vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n) {
	return 
}

inline bool Approximately(float val1, float val2) {
	return (std::fabs(val1 - val2) < FLT_EPSILON);
}

inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y) {
	float omt = 1 - t;

	float a1 = omt * omt;
	float b1 = 2 * omt * t;
	float c1 = t * t;

	x = (int)(a1 * x1 + b1 * x2 + c1 * x3);
	y = (int)(a1 * y1 + b1 * y2 + c1 * y3);
}

inline void CubicPoint(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float t, int& x, int& y) {
	float omt = 1 - t;
 
	float a = omt * omt * omt;
	float b = 3 * (omt * omt) * t;
	float c = 3 * omt * (t * t);
	float d = t * t * t;
 
	x = (int)((a * x1) + (b * x2) + (c * x3) + (d * x4));
	y = (int)((a * y1) + (b * y2) + (c * y3) + (d * y4));
}