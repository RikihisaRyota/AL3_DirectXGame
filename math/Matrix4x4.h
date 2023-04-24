#pragma once
#include "mat4x4.h"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

Matrix4x4 Convert(const mat4x4& m1);