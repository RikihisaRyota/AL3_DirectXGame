#pragma once
#include "Collider.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include <Input.h>
#include <list>

class GameScene;
/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider {
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
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection& viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	Matrix4x4 GetMatWorld() { return worldTransform_.matWorld_; }
	Matrix4x4 GetParentMatWorld() { return worldTransform_.parent_->matWorld_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	// 弾のリストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	// 半径ゲッター
	float GetRadius() override { return radius_; }

	// セッター
	void SetRadius(float radius) override { radius_ = radius; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) {
		worldTransform_.parent_ = parent;
		//worldTransform3DReticle_.parent_ = parent;
	}

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;
	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	// 半径
	float radius_ = 1.0f;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;
	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	// 3Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	// レティクルポジション(screen)
	Vector3 reticlePosition_;
	// 
	std::vector<Vector3> controlPoints_;
	// catmull
	int catmullCount = 51;
	// 線分を描画する用の頂点リスト
	std::vector<Vector3> points;
	// 線分の数
	const size_t segmentCount = 200;
};