#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator-(Vector2 other) const { return Vector2(x - other.x, y - other.y); }
	Vector2 operator+(Vector2 other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 operator*(Vector2 other) const { return Vector2(x * other.x, y * other.y); }
	Vector2 operator*(float other) const { return Vector2(x * other, y * other); }
};
