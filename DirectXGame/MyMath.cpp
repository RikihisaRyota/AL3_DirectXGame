#include "MyMath.h"

#include <cassert>
#include <cmath>
#include <list>
#include <vector>

#include "Vector3.h"

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

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 mat;
	mat.m[0][0] = m1.m[0][0] + m2.m[0][0], mat.m[0][1] = m1.m[0][1] + m2.m[0][1],
	mat.m[0][2] = m1.m[0][2] + m2.m[0][2], mat.m[0][3] = m1.m[0][3] + m2.m[0][3];
	mat.m[1][0] = m1.m[1][0] + m2.m[1][0], mat.m[1][1] = m1.m[1][1] + m2.m[1][1],
	mat.m[1][2] = m1.m[1][2] + m2.m[1][2], mat.m[1][3] = m1.m[1][3] + m2.m[1][3];
	mat.m[2][0] = m1.m[2][0] + m2.m[2][0], mat.m[2][1] = m1.m[2][1] + m2.m[2][1],
	mat.m[2][2] = m1.m[2][2] + m2.m[2][2], mat.m[2][3] = m1.m[2][3] + m2.m[2][3];
	mat.m[3][0] = m1.m[3][0] + m2.m[3][0], mat.m[3][1] = m1.m[3][1] + m2.m[3][1],
	mat.m[3][2] = m1.m[3][2] + m2.m[3][2], mat.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return mat;
}

Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 mat;
	mat.m[0][0] = m1.m[0][0] - m2.m[0][0], mat.m[0][1] = m1.m[0][1] - m2.m[0][1],
	mat.m[0][2] = m1.m[0][2] - m2.m[0][2], mat.m[0][3] = m1.m[0][3] - m2.m[0][3];
	mat.m[1][0] = m1.m[1][0] - m2.m[1][0], mat.m[1][1] = m1.m[1][1] - m2.m[1][1],
	mat.m[1][2] = m1.m[1][2] - m2.m[1][2], mat.m[1][3] = m1.m[1][3] - m2.m[1][3];
	mat.m[2][0] = m1.m[2][0] - m2.m[2][0], mat.m[2][1] = m1.m[2][1] - m2.m[2][1],
	mat.m[2][2] = m1.m[2][2] - m2.m[2][2], mat.m[2][3] = m1.m[2][3] - m2.m[2][3];
	mat.m[3][0] = m1.m[3][0] - m2.m[3][0], mat.m[3][1] = m1.m[3][1] - m2.m[3][1],
	mat.m[3][2] = m1.m[3][2] - m2.m[3][2], mat.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return mat;
}

Matrix4x4 Mul(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 mat;
	mat.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	              m1.m[0][3] * m2.m[3][0],
	mat.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	              m1.m[0][3] * m2.m[3][1],
	mat.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	              m1.m[0][3] * m2.m[3][2],
	mat.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	              m1.m[0][3] * m2.m[3][3],

	mat.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	              m1.m[1][3] * m2.m[3][0],
	mat.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	              m1.m[1][3] * m2.m[3][1],
	mat.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	              m1.m[1][3] * m2.m[3][2],
	mat.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	              m1.m[1][3] * m2.m[3][3],

	mat.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	              m1.m[2][3] * m2.m[3][0],
	mat.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	              m1.m[2][3] * m2.m[3][1],
	mat.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	              m1.m[2][3] * m2.m[3][2],
	mat.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	              m1.m[2][3] * m2.m[3][3],

	mat.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	              m1.m[3][3] * m2.m[3][0],
	mat.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	              m1.m[3][3] * m2.m[3][1],
	mat.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	              m1.m[3][3] * m2.m[3][2],
	mat.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	              m1.m[3][3] * m2.m[3][3];

	return mat;
}

Matrix4x4 Mul(const float scaler, const Matrix4x4& m2) {
	Matrix4x4 mat;
	mat.m[0][0] = scaler * m2.m[0][0], mat.m[0][1] = scaler * m2.m[0][1],
	mat.m[0][2] = scaler * m2.m[0][2], mat.m[0][3] = scaler * m2.m[0][3];
	mat.m[1][0] = scaler * m2.m[1][0], mat.m[1][1] = scaler * m2.m[1][1],
	mat.m[1][2] = scaler * m2.m[1][2], mat.m[1][3] = scaler * m2.m[1][3];
	mat.m[2][0] = scaler * m2.m[2][0], mat.m[2][1] = scaler * m2.m[2][1],
	mat.m[2][2] = scaler * m2.m[2][2], mat.m[2][3] = scaler * m2.m[2][3];
	mat.m[3][0] = scaler * m2.m[3][0], mat.m[3][1] = scaler * m2.m[3][1],
	mat.m[3][2] = scaler * m2.m[3][2], mat.m[3][3] = scaler * m2.m[3][3];
	return mat;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	float A;
	A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	    m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	    m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
	    m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	    m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
	    m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	    m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
	    m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	    m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	    m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	    m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	    m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
	    m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	    m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
	    m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	    m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
	    m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	    m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	    m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 tmp;
	tmp.m[0][0] = 1.0f / A *
	              (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	               m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	               m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);

	tmp.m[0][1] = 1.0f / A *
	              (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	               m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	               m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);

	tmp.m[0][2] = 1.0f / A *
	              (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	               m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	               m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);

	tmp.m[0][3] = 1.0f / A *
	              (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	               m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	               m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	tmp.m[1][0] = 1.0f / A *
	              (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	               m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	               m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);

	tmp.m[1][1] = 1.0f / A *
	              (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	               m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	               m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);

	tmp.m[1][2] = 1.0f / A *
	              (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	               m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	               m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);

	tmp.m[1][3] = 1.0f / A *
	              (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	               m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	               m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

	tmp.m[2][0] = 1.0f / A *
	              (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	               m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	               m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);

	tmp.m[2][1] = 1.0f / A *
	              (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	               m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	               m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);

	tmp.m[2][2] = 1.0f / A *
	              (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	               m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	               m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);

	tmp.m[2][3] = 1.0f / A *
	              (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	               m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	               m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

	tmp.m[3][0] = 1.0f / A *
	              (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	               m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	               m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);

	tmp.m[3][1] = 1.0f / A *
	              (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	               m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	               m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);

	tmp.m[3][2] = 1.0f / A *
	              (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	               m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	               m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);

	tmp.m[3][3] = 1.0f / A *
	              (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	               m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	               m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return tmp;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 mat;
	mat.m[0][0] = m.m[0][0], mat.m[0][1] = m.m[1][0], mat.m[0][2] = m.m[2][0],
	mat.m[0][3] = m.m[3][0];
	mat.m[1][0] = m.m[0][1], mat.m[1][1] = m.m[1][1], mat.m[1][2] = m.m[2][1],
	mat.m[1][3] = m.m[3][1];
	mat.m[2][0] = m.m[0][2], mat.m[2][1] = m.m[1][2], mat.m[2][2] = m.m[2][2],
	mat.m[2][3] = m.m[3][2];
	mat.m[3][0] = m.m[0][3], mat.m[3][1] = m.m[1][3], mat.m[3][2] = m.m[2][3],
	mat.m[3][3] = m.m[3][3];
	return mat;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 mat;
	mat.m[0][0] = 1.0f, mat.m[0][1] = 0.0f, mat.m[0][2] = 0.0f, mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = 1.0f, mat.m[1][2] = 0.0f, mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = 0.0f, mat.m[2][2] = 1.0f, mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f, mat.m[3][1] = 0.0f, mat.m[3][2] = 0.0f, mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 tmp;
	tmp.m[0][0] = 1.0f, tmp.m[0][1] = 0.0f, tmp.m[0][2] = 0.0f, tmp.m[0][3] = 0.0f;
	tmp.m[1][0] = 0.0f, tmp.m[1][1] = 1.0f, tmp.m[1][2] = 0.0f, tmp.m[1][3] = 0.0f;
	tmp.m[2][0] = 0.0f, tmp.m[2][1] = 0.0f, tmp.m[2][2] = 1.0f, tmp.m[2][3] = 0.0f;
	tmp.m[3][0] = translate.x, tmp.m[3][1] = translate.y, tmp.m[3][2] = translate.z,
	tmp.m[3][3] = 1.0f;
	return tmp;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 tmp;
	tmp.m[0][0] = scale.x, tmp.m[0][1] = 0.0f, tmp.m[0][2] = 0.0f, tmp.m[0][3] = 0.0f;
	tmp.m[1][0] = 0.0f, tmp.m[1][1] = scale.y, tmp.m[1][2] = 0.0f, tmp.m[1][3] = 0.0f;
	tmp.m[2][0] = 0.0f, tmp.m[2][1] = 0.0f, tmp.m[2][2] = scale.z, tmp.m[2][3] = 0.0f;
	tmp.m[3][0] = 0.0f, tmp.m[3][1] = 0.0f, tmp.m[3][2] = 0.0f, tmp.m[3][3] = 1.0f;
	return tmp;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{0.0f, 0.0f, 0.0f};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 mat;
	mat.m[0][0] = 1.0f, mat.m[0][1] = 0.0f, mat.m[0][2] = 0.0f, mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = std::cos(radian), mat.m[1][2] = std::sin(radian),
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = -std::sin(radian), mat.m[2][2] = std::cos(radian),
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f, mat.m[3][1] = 0.0f, mat.m[3][2] = 0.0f, mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 mat;
	mat.m[0][0] = std::cos(radian), mat.m[0][1] = 0.0f, mat.m[0][2] = -std::sin(radian),
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = 1.0f, mat.m[1][2] = 0.0f, mat.m[1][3] = 0.0f;
	mat.m[2][0] = std::sin(radian), mat.m[2][1] = 0.0f, mat.m[2][2] = std::cos(radian),
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f, mat.m[3][1] = 0.0f, mat.m[3][2] = 0.0f, mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 mat;
	mat.m[0][0] = std::cos(radian), mat.m[0][1] = std::sin(radian), mat.m[0][2] = 0.0f,
	mat.m[0][3] = 0.0f;
	mat.m[1][0] = -std::sin(radian), mat.m[1][1] = std::cos(radian), mat.m[1][2] = 0.0f,
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = 0.0f, mat.m[2][2] = 1.0f, mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f, mat.m[3][1] = 0.0f, mat.m[3][2] = 0.0f, mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 tmp = Mul(
	    MakeRotateXMatrix(rotate.x), Mul(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	Matrix4x4 mat;
	mat.m[0][0] = scale.x * tmp.m[0][0], mat.m[0][1] = scale.x * tmp.m[0][1],
	mat.m[0][2] = scale.x * tmp.m[0][2], mat.m[0][3] = 0.0f;
	mat.m[1][0] = scale.y * tmp.m[1][0], mat.m[1][1] = scale.y * tmp.m[1][1],
	mat.m[1][2] = scale.y * tmp.m[1][2], mat.m[1][3] = 0.0f;
	mat.m[2][0] = scale.z * tmp.m[2][0], mat.m[2][1] = scale.z * tmp.m[2][1],
	mat.m[2][2] = scale.z * tmp.m[2][2], mat.m[2][3] = 0.0f;
	mat.m[3][0] = translate.x, mat.m[3][1] = translate.y, mat.m[3][2] = translate.z,
	mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 mat;
	mat.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2.0f)), mat.m[0][1] = 0.0f,
	mat.m[0][2] = 0.0f, mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = 1.0f / std::tan(fovY / 2.0f), mat.m[1][2] = 0.0f,
	mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = 0.0f, mat.m[2][2] = farClip / (farClip - nearClip),
	mat.m[2][3] = 1.0f;
	mat.m[3][0] = 0.0f, mat.m[3][1] = 0.0f,
	mat.m[3][2] = (-nearClip * farClip) / (farClip - nearClip), mat.m[3][3] = 0.0f;
	return mat;
}

Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 mat;
	mat.m[0][0] = 2.0f / (right - left), mat.m[0][1] = 0.0f, mat.m[0][2] = 0.0f, mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = 2.0f / (top - bottom), mat.m[1][2] = 0.0f, mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = 0.0f, mat.m[2][2] = 1.0f / (farClip - nearClip),
	mat.m[2][3] = 0.0f;
	mat.m[3][0] = (left + right) / (left - right), mat.m[3][1] = (top + bottom) / (bottom - top),
	mat.m[3][2] = nearClip / (nearClip - farClip), mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 mat;
	mat.m[0][0] = width / 2.0f, mat.m[0][1] = 0.0f, mat.m[0][2] = 0.0f, mat.m[0][3] = 0.0f;
	mat.m[1][0] = 0.0f, mat.m[1][1] = -height / 2.0f, mat.m[1][2] = 0.0f, mat.m[1][3] = 0.0f;
	mat.m[2][0] = 0.0f, mat.m[2][1] = 0.0f, mat.m[2][2] = maxDepth - minDepth, mat.m[2][3] = 0.0f;
	mat.m[3][0] = left + (width / 2.0f), mat.m[3][1] = top + (height / 2.0f),
	mat.m[3][2] = minDepth, mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 Convert(const Matrix4x4& m1) {
	Matrix4x4 mat;
	mat.m[0][0] = m1.m[0][0], mat.m[0][1] = m1.m[0][1], mat.m[0][2] = m1.m[0][2],
	mat.m[0][3] = m1.m[0][3];
	mat.m[1][0] = m1.m[1][0], mat.m[1][1] = m1.m[1][1], mat.m[1][2] = m1.m[1][2],
	mat.m[1][3] = m1.m[1][3];
	mat.m[2][0] = m1.m[2][0], mat.m[2][1] = m1.m[2][1], mat.m[2][2] = m1.m[2][2],
	mat.m[2][3] = m1.m[2][3];
	mat.m[3][0] = m1.m[3][0], mat.m[3][1] = m1.m[3][1], mat.m[3][2] = m1.m[3][2],
	mat.m[3][3] = m1.m[3][3];
	return mat;
}