#pragma once
#include "Matrix4x4.h"
#include "WorldTransform.h"
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

Vector3 Slerp(const Vector3& start, const Vector3& end, float t);
