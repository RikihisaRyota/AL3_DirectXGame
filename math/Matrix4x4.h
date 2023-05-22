#pragma once
#include "mat4x4.h"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	Matrix4x4& operator*=(const Matrix4x4& other) {
		Matrix4x4 temp = *this;
		m[0][0] = temp.m[0][0] * other.m[0][0] + temp.m[0][1] * other.m[1][0] +
		          temp.m[0][2] * other.m[2][0] + temp.m[0][3] * other.m[3][0];
		m[0][1] = temp.m[0][0] * other.m[0][1] + temp.m[0][1] * other.m[1][1] +
		          temp.m[0][2] * other.m[2][1] + temp.m[0][3] * other.m[3][1];
		m[0][2] = temp.m[0][0] * other.m[0][2] + temp.m[0][1] * other.m[1][2] +
		          temp.m[0][2] * other.m[2][2] + temp.m[0][3] * other.m[3][2];
		m[0][3] = temp.m[0][0] * other.m[0][3] + temp.m[0][1] * other.m[1][3] +
		          temp.m[0][2] * other.m[2][3] + temp.m[0][3] * other.m[3][3];
		m[1][0] = temp.m[1][0] * other.m[0][0] + temp.m[1][1] * other.m[1][0] +
		          temp.m[1][2] * other.m[2][0] + temp.m[1][3] * other.m[3][0];
		m[1][1] = temp.m[1][0] * other.m[0][1] + temp.m[1][1] * other.m[1][1] +
		          temp.m[1][2] * other.m[2][1] + temp.m[1][3] * other.m[3][1];
		m[1][2] = temp.m[1][0] * other.m[0][2] + temp.m[1][1] * other.m[1][2] +
		          temp.m[1][2] * other.m[2][2] + temp.m[1][3] * other.m[3][2];
		m[1][3] = temp.m[1][0] * other.m[0][3] + temp.m[1][1] * other.m[1][3] +
		          temp.m[1][2] * other.m[2][3] + temp.m[1][3] * other.m[3][3];
		m[2][0] = temp.m[2][0] * other.m[0][0] + temp.m[2][1] * other.m[1][0] +
		          temp.m[2][2] * other.m[2][0] + temp.m[2][3] * other.m[3][0];
		m[2][1] = temp.m[2][0] * other.m[0][1] + temp.m[2][1] * other.m[1][1] +
		          temp.m[2][2] * other.m[2][1] + temp.m[2][3] * other.m[3][1];
		m[2][2] = temp.m[2][0] * other.m[0][2] + temp.m[2][1] * other.m[1][2] +
		          temp.m[2][2] * other.m[2][2] + temp.m[2][3] * other.m[3][2];
		m[2][3] = temp.m[2][0] * other.m[0][3] + temp.m[2][1] * other.m[1][3] +
		          temp.m[2][2] * other.m[2][3] + temp.m[2][3] * other.m[3][3];
		m[3][0] = temp.m[3][0] * other.m[0][0] + temp.m[3][1] * other.m[1][0] +
		          temp.m[3][2] * other.m[2][0] + temp.m[3][3] * other.m[3][0];
		m[3][1] = temp.m[3][0] * other.m[0][1] + temp.m[3][1] * other.m[1][1] +
		          temp.m[3][2] * other.m[2][1] + temp.m[3][3] * other.m[3][1];
		m[3][2] = temp.m[3][0] * other.m[0][2] + temp.m[3][1] * other.m[1][2] +
		          temp.m[3][2] * other.m[2][2] + temp.m[3][3] * other.m[3][2];
		m[3][3] = temp.m[3][0] * other.m[0][3] + temp.m[3][1] * other.m[1][3] +
		          temp.m[3][2] * other.m[2][3] + temp.m[3][3] * other.m[3][3];

		return *this;
	}

	Matrix4x4& operator+=(const Matrix4x4& other) {
		m[0][0] += other.m[0][0];
		m[0][1] += other.m[0][1];
		m[0][2] += other.m[0][2];
		m[0][3] += other.m[0][3];
		m[1][0] += other.m[1][0];
		m[1][1] += other.m[1][1];
		m[1][2] += other.m[1][2];
		m[1][3] += other.m[1][3];
		m[2][0] += other.m[2][0];
		m[2][1] += other.m[2][1];
		m[2][2] += other.m[2][2];
		m[2][3] += other.m[2][3];
		m[3][0] += other.m[3][0];
		m[3][1] += other.m[3][1];
		m[3][2] += other.m[3][2];
		m[3][3] += other.m[3][3];
		return *this;
	}

	Matrix4x4 operator*(const Matrix4x4& other) const {
		Matrix4x4 result;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float sum = 0.0f;
				for (int k = 0; k < 4; k++) {
					sum += m[i][k] * other.m[k][j];
				}
				result.m[i][j] = sum;
			}
		}

		return result;
	}
};

Matrix4x4 Convert(const mat4x4& m1);