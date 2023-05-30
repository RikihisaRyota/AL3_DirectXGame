#pragma once
#include <list>
#include <vector>
#include "Vector3.h"
#include "ViewProjection.h"
#include "PrimitiveDrawer.h"

class Catmull_Rom {
public:
	// 初期化
	void Initialize();

	// 描画
	void Draw(const ViewProjection& viewProjection);

public:
	PrimitiveDrawer* drawer_ = nullptr;
	std::vector<Vector3> controlPoints_;
};
