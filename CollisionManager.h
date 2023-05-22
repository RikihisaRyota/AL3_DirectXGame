#pragma once
#include <list>
#include <memory>
#include "Collider.h"
class Player;
class Enemy;
class EnemyBullet;

class CollisionManager {
public:
	void Update(
	    Player* player, const std::list<std::unique_ptr<Enemy>>& enemy,
	    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet);

private:
	// メンバ関数
	void CheckAllCollisions(
	    Player* player, const std::list<std::unique_ptr<Enemy>>& enemy,
	    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet);

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheakCollisionPair(Collider* colliderA, Collider* colliderB);
	// コライダー
	std::list<Collider*> colliders_;
};
