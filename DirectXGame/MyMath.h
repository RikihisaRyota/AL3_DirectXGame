#pragma once
#include <algorithm> // std::min, std::max

#include "Matrix4x4.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vector4.h"

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
float Lerp(float start, float end, float t);

Vector3 Slerp(const Vector3& start, const Vector3& end, float t);

float Distance(const Vector3& v1, const Vector3& v2);

Vector3 Normalize(const Vector3& v1);

Vector3 CatmullRom(Vector3 point0, Vector3 point1, Vector3 point2, Vector3 point3, float t);

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
// スクリーン変換行列
Matrix4x4 MakeViewProjectMatrixMatrix(const ViewProjection& viewProjection);

Matrix4x4 Convert(const Matrix4x4& m1);

/// <summary>
/// Clamp
/// </summary>
/// <param name="num"></param>
/// <param name="min">min</param>
/// <param name="max">max</param>
/// <returns></returns>
float Clamp(float num, float min, float max);
/// <summary>
/// radian->degree
/// </summary>
/// <param name="radian">radian</param>
/// <returns>degree</returns>
float RadToDeg(float radian);
/// <summary>
/// degree->radian
/// </summary>
/// <param name="degree">degree</param>
/// <returns>radian</returns>
float DegToRad(float degree);
/// <summary> 
/// ワールドトランスフォームからワールド行列を生成
/// </summary>
/// /// <param name="worldtransform"></param>
/// /// <returns></returns>
Matrix4x4 MakeMatWolrd(const WorldTransform& worldtransform);

/// <summary>
/// ヒットボックス表示
/// </summary>
/// <param name="worldtransform">worldtransform</param>
/// <param name="viewProjection">viewProjection</param>
/// <param name="color">color</param>
void ChackHitBox(
    const WorldTransform& worldtransform, const ViewProjection& viewProjection,
    const Vector4& color);