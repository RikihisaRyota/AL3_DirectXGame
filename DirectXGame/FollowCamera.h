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

private: 
	// 回転速度
	const float kRotateSpeedX = 0.000001f;
	const float kRotateSpeedY = 0.000002f;
	// 上下方向の回転のデッドゾーン
	const float deadZoneRotateMin = -10.0f;
	const float deadZoneRotateMax = 70.0f;
private: // メンバ変数
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 追従対象
	const WorldTransform* target_ = nullptr;
};
