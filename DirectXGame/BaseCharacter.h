#pragma once
#include <memory>
#include <vector>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータの配列</param>
	virtual void Initialize(std::vector<std::unique_ptr<Model>> model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	virtual void Draw(const ViewProjection& viewProjection);
public:
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
protected:
	// モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ワールドトランスフォーム(アニメーション用)
	WorldTransform worldTransform_Motion_;
	// ワールドトランスフォームパーツごと
	std::vector<WorldTransform> worldTransforms_Parts_;
	// ベクトル
	Vector3 vector_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
};