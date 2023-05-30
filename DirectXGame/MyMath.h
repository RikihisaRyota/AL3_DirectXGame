#pragma once
#include "Matrix4x4.h"
#include "WorldTransform.h"
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

Vector3 Slerp(const Vector3& start, const Vector3& end, float t);

float Distance(const Vector3& v1, const Vector3& v2);

//Vector3 operator*(const float a, const Vector3& v) { return Vector3(a * v.x, a * v.y, a * v.z); }

Vector3 CatmullRom(Vector3 point0,Vector3 point1,Vector3 point2,Vector3 point3,float t);