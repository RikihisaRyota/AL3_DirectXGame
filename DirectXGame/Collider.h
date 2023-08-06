#pragma once
#include <cstdint>

#include "AABB.h"
#include "Sphere.h"
#include "OBB.h"

// プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
// 敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
struct Vector3;
/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	// 衝突時に呼ばれる関数
	virtual void OnCollision(const AABB& aabb) = 0;

	// 衝突属性の取得
	virtual uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	// 衝突属性設定
	virtual void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}

	// 衝突マスクの取得
	virtual uint32_t GetCollisionMask() { return collisionAttribute_; }

	// 衝突マスク設定
	virtual void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	AABB* GetAABB() { return &aabb_; }
	Sphere* GetSphere() { return &sphere_; }

protected:
	// AABB
	AABB aabb_;
	// OBB
	OBB obb_;
	// Sphere
	Sphere sphere_;
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
