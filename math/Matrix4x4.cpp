#include "Matrix4x4.h"

Matrix4x4 Convert(const mat4x4& m1) {
	Matrix4x4 mat;
	mat.m[0][0] = m1.m[0][0], mat.m[0][1] = m1.m[0][1], mat.m[0][2] = m1.m[0][2],
	mat.m[0][3] = m1.m[0][3];
	mat.m[1][0] = m1.m[1][0], mat.m[1][1] = m1.m[1][1], mat.m[1][2] = m1.m[1][2],
	mat.m[1][3] = m1.m[1][3];
	mat.m[2][0] = m1.m[2][0], mat.m[2][1] = m1.m[2][1], mat.m[2][2] = m1.m[2][2],
	mat.m[2][3] = m1.m[2][3];
	mat.m[3][0] = m1.m[3][0], mat.m[3][1] = m1.m[3][1], mat.m[3][2] = m1.m[3][2],
	mat.m[3][3] = m1.m[3][3];
	return mat;
}
