#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行列を計算、転送する
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	Vector3 GetWorldPosition() override { return worldTransform_.translation_; }

	float GetRadius() override { return radius_; }

	// セッター
	void SetRadius(float radius) override { radius_ = radius; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	//半径
	float radius_ = 1.0f;
};
