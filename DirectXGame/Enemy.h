#pragma once
#include "BaseCharacter.h"
class Enemy : public BaseCharacter {
public:
	// パーツ
	enum class Parts { 
		BODY, 
		LIGHT, 
		COUNT
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>> model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;
};
