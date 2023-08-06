#pragma once

#include "Vector3.h"

struct Line {
	Vector3 origin_; //!< 始点
	Vector3 diff_;   //!< 終点への差分ベクトル
};

struct Ray {
	Vector3 origin_; //!< 始点
	Vector3 diff_;   //!< 終点への差分ベクトル
};

struct Segment {
	Vector3 origin_; //!< 始点
	Vector3 diff_;   //!< 終点への差分ベクトル
};
