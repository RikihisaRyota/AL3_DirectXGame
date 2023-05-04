#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"
/// <summary>
/// 自キャラ
/// </summary>
class Player:public Collider {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model,uint32_t textureHandle);

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
	/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	// 弾のリストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//半径ゲッター
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
	//キーボード入力
	Input* input_ = nullptr;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//半径
	float radius_ = 1.0f;
};
