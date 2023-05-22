#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
struct Vector3;
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3* position, const Vector3* radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection* viewProjection_ /*Vector3* position, Vector3* radian*/);

	/// <summary>
	/// 描画(デバック用)
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドマトリックスゲッター
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};
