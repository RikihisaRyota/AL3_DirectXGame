#pragma once
#include "Vector3.h"
/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision(){};

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0; 

	// 半径の取得
	virtual float GetRadius() { return radius_; }

	// 半径の設定
	virtual void SetRadius(float radius) { radius_ = radius; }

private:
	
	// 衝突半径
	float radius_;
};
