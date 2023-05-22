#include "Vector2.h"

Vector2::Vector2() { x = 0.0f, y = 0.0f; }

Vector2::Vector2(float a, float b) { x = a, y = b; }

Vector2 Vector2::operator+() const { return *this; }

Vector2 Vector2::operator-() const { return {-x, -y}; }

Vector2 Vector2::operator=(const Vector2& other) const { return {other.x, other.y}; }

Vector2 Vector2::operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }

Vector2 Vector2::operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }

Vector2 Vector2::operator*(const Vector2& other) const { return {x * other.x, y * other.y}; }

Vector2 Vector2::operator*(float s) const { return {x * s, y * s}; }

Vector2 Vector2::operator/(float s) const { return {x * 1 / s, y * 1 / s}; }

Vector2& Vector2::operator+=(const Vector2& other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

Vector2 operator*(float s, const Vector2& v) { return {s * v.x, s * v.y}; }