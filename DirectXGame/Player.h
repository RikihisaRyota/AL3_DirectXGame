#pragma once
#include <cstdlib>
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Player {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::unique_ptr<Model> model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ関数
	/// <summary>
	/// プレイヤーの動き
	/// </summary>
	void Move();

private: // メンバ変数
	// ワールド
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
	// 速さ
	const float kSpeed = 0.3f;
};