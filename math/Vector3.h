#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& a) {
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}
};