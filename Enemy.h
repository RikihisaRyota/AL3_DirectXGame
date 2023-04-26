#pragma once
#include "Model.h"
#include "WorldTransform.h"
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
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
private:
	// フェーズごとのアップデート
	void UpdateApproach();
	void UpdateLeave();

	// メンバ関数ポインタのテーブル
	static void (Enemy::*spUpdateTable[])();
	// ワールド変換データ
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// モデル
	Model* model_ = nullptr;
	//行動フェーズ
	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};
	//フェーズ
	Phase phase_=Phase::Approach;
};
