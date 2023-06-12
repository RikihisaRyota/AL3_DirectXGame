#pragma once
#include <list>
#include <vector>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "PrimitiveDrawer.h"

struct Vector3;
class Player;
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3* position, const Vector3* radian,Player* player);

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
	// プレイヤー借りてくる
	Player* player_;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// 
	PrimitiveDrawer* drawer_ = nullptr;
	std::vector<Vector3> controlPoints_;
	// catmull
	int catmullCount = 4;
	// 線分を描画する用の頂点リスト
	std::vector<Vector3> points;
	// 線分の数
	const size_t segmentCount = 400;
};
