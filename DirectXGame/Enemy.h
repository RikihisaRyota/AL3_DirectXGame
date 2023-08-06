#pragma once
#include "BaseCharacter.h"
class Enemy : public BaseCharacter, public Collider {
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
	void Draw(const ViewProjection& viewProjection);

	void HitBoxDraw(const ViewProjection& viewProjection);

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 動作
	/// </summary>
	void Motion();
	/// <summary>
	/// 全体
	/// </summary>
	void Base();
	/// <summary>
	/// 体
	/// </summary>
	void Body();
	/// <summary>
	/// ライト
	/// </summary>
	void Light();

	void HitBoxUpdate();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision(const AABB& aabb) override;

private:
	// 振り向き速度
	const float kTurn = 0.4f;

private:
	// 向き
	Vector3 direction_;
	// 移動回転用
	float moveRatate_;
	// 動作回転用
	float motionRatate_;
	/////////
	float angle_ = 0.0f;
};
