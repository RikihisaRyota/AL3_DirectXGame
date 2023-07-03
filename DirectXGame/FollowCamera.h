#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
class FollowCamera {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Intialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // セッター,ゲッター
	/// <summary>
	/// ターゲット
	/// </summary>
	/// <param name="target">target</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	ViewProjection* GetViewProjection() { return &viewProjection_; }

private: // メンバ変数
	// 回転速度
	const float kRotateSpeed = 0.000001f;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
};
