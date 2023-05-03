#pragma once
#include <assert.h>
#include <cmath>

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

	Vector3& operator-=(const Vector3& a) {
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}

	Vector3 operator+(const Vector3& a) const {
		return Vector3(this->x + a.x, this->y + a.y, this->z + a.z);
	}

	Vector3 operator-(const Vector3& a) const {
		return Vector3(this->x - a.x, this->y - a.y, this->z - a.z);
	}

	Vector3 operator*(const Vector3& a) const {
		return Vector3(this->x * a.x, this->y * a.y, this->z * a.z);
	}

	Vector3 operator*(const float a) const {
		return Vector3(this->x * a, this->y * a, this->z * a);
	}


	void Normalize() {
		float length = sqrt(x * x + y * y + z * z);
		assert(length != 0);
		x /= length;
		y /= length;
		z /= length;
	}
};
