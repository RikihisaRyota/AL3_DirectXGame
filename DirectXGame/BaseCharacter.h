#pragma once
#include <vector>
#include <memory>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class BaseCharacter {
protected:
	// モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータの配列</param>
	virtual void Initialize(const std::vector<std::unique_ptr<Model>> models);
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	virtual void Draw(const ViewProjection& viewProjection);
};
