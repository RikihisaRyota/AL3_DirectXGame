#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cstdlib>
#include <memory>

class Player {
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::unique_ptr<Model> model, int32_t texture = 0);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ変数
	// ワールド
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
	// テクスチャハンドル
	int32_t texture_;
};