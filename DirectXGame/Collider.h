#pragma once
#include <cstdint>


//プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
struct Vector3;
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

	// 衝突属性の取得
	virtual uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	// 衝突属性設定
	virtual void SetCollisionAttribute(uint32_t collisionAttribute) {collisionAttribute_ = collisionAttribute; }

	// 衝突マスクの取得
	virtual uint32_t GetCollisionMask() { return collisionAttribute_; }

	// 衝突マスク設定
	virtual void SetCollisionMask(uint32_t collisionMask) {
		collisionMask_ = collisionMask;
	}

private:
	// 衝突半径
	float radius_;
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
