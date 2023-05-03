#include "MyMath.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	{ return start + ((end - start) * t); }
}

Vector3 Slerp(const Vector3& start, const Vector3& end, float t) {
	float omega = std::acos(start.x * end.x + start.y * end.y + start.z * end.z);
	float so = std::sin(omega);
	if (so == 0.0f) {
		return start;
	}
	return start * (std::sin((1 - t) * omega) / so) + end * (std::sin(t * omega) / so);
}
