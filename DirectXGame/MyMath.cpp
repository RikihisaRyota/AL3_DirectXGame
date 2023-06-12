#include "MyMath.h"
#include "Vector3.h"
#include <list>
#include <vector>

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t) {
	return start + ((end - start) * t);
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	// 2つのベクトルの内積を計算
	const float cosTheta = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	// 角度（ラジアン）を計算
	const float theta = std::acos(cosTheta);

	// sin(x) / sin(theta) を計算するために、thetaが0に近い場合は線形補間を行う
	if (std::abs(theta) < 0.0001f) {
		return v1 * (1.0f - t) + v2 * t;
	}

	// sin(theta) を計算
	const float sinTheta = std::sin(theta);

	  // 0除算による計算不可を回避するため、sinThetaが0に近い場合は線形補間を行う
	if (std::abs(sinTheta) < 0.0001f) {
		return v1 * (1.0f - t) + v2 * t;
	}

	// パラメータによって補間するベクトルを計算
	const float w1 = std::sin((1.0f - t) * theta) / sinTheta;
	const float w2 = std::sin(t * theta) / sinTheta;
	const Vector3 result = v1 * w1 + v2 * w2;

	return result;
}

Vector3 CatmullRom(Vector3 Position0, Vector3 Position1, Vector3 Position2, Vector3 Position3, float t) {
	Vector3 Result;

	float t2 = t * t;
	float t3 = t2 * t;

	float P0 = -t3 + 2.0f * t2 - t;
	float P1 = 3.0f * t3 - 5.0f * t2 + 2.0f;
	float P2 = -3.0f * t3 + 4.0f * t2 + t;
	float P3 = t3 - t2;

	Result.x = (P0 * Position0.x + P1 * Position1.x + P2 * Position2.x + P3 * Position3.x) * 0.5f;
	Result.y = (P0 * Position0.y + P1 * Position1.y + P2 * Position2.y + P3 * Position3.y) * 0.5f;
	Result.z = (P0 * Position0.z + P1 * Position1.z + P2 * Position2.z + P3 * Position3.z) * 0.5f;

	return Result;
}

float Distance(const Vector3& v1, const Vector3& v2) {
	return static_cast<float>(
	    std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2) + std::pow(v2.z - v1.z, 2));
}
