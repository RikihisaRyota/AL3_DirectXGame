#pragma once

#include "ViewProjection.h"
class FollowCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Intialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
};
