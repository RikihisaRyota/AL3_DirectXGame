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

// 1,行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 2,行列の減算
Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2);
// 3,行列の積
Matrix4x4 Mul(const Matrix4x4& m1, const Matrix4x4& m2);
// 3,行列の積(スカラー倍)
Matrix4x4 Mul(const float scaler, const Matrix4x4& m2);
// 4,逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 5,転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 6,単位行列
Matrix4x4 MakeIdentity4x4();

// 1,平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 2,拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 3,座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 1,X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2,Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3,Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 3次元アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 1,透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// 2,正射影行列
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
// 3,ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 Convert(const Matrix4x4& m1);