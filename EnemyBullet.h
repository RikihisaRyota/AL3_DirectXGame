#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"

class Player;
/// <summary>
/// 敵の弾
/// </summary>
class EnemyBullet:public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行列を計算転送する
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override { return worldTransform_.translation_; }

	// 半径ゲッター
	float GetRadius() override { return radius_; }

	// セッター
	void SetRadius(float radius) override { radius_ = radius; }



private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_= 0u;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	//自キャラ
	Player* player_=nullptr;
	//弾の半径
	float radius_ = 1.0f;
};
