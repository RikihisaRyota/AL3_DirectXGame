#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyState.h"
#include "EnemyBullet.h"
#include "TimeCall.h"
#include <list>
// 自機クラスの前方宣言
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

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
	/// WorldTransformのゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// translationのゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetTanslation() { return worldTransform_.translation_; }

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	void AddTanslation(const Vector3& position) { worldTransform_.translation_ += position; }

	/// <summary>
	/// changeState
	/// </summary>
	void changeState(EnemyState* newState);

	/// <summary>
	/// 弾を発射しタイマーをリセットするコールバック関数
	/// </summary>
	void BulletsSet();

	/// <summary>
	/// 弾
	/// </summary>
	void Fire();

	//発射間隔
	static const int kFireInterval = 60;

	// 自キャラ
	Player* player_ = nullptr;

	void SetPlayer(Player* player) { player_ = player; }

private:
	/// <summary>
	/// 行動パターン更新
	/// </summary>
	void UpdateState();

	// フェーズごとのアップデート
	EnemyState* state_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// モデル
	Model* model_ = nullptr;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//発射タイマー
	int32_t fireTimer_ = 0;
	//時限発動リスト
	std::list<std::unique_ptr<TimeCall>> timedCalls_;
};
