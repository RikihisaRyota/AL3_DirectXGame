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

public: // ゲッター,セッター
	/// <summary>
	/// WorldTransformのゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
	/// <summary>
	/// ViewProjectionのセッター
	/// </summary>
	/// <param name="viewprojection"></param>
	void SetViewProjection(const ViewProjection* viewprojection) {
		viewProjection_ = viewprojection;
	}
private: // 定数系
	// キャラクターの速さ
	const float kSpeed = 0.3f;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
private: // メンバ変数
	// ワールド
	WorldTransform worldTransform_;
	// モデル
	std::unique_ptr<Model> model_;
};