#include "CollisionManager.h"
#include "Vector3.h"
#include "MyMath.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"


void CollisionManager::Update(Player* player,Enemy* enemy) { 
	colliders_.clear();
	CheckAllCollisions(player,enemy); 
}


void CollisionManager::CheckAllCollisions(Player* player_, Enemy* enemy_) {
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	// コライダーをリストに追加
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);

	// 自弾全てについて
	for (std::list<std::unique_ptr<PlayerBullet>>::const_iterator it = playerBullets.begin();
	     it != playerBullets.end(); ++it) {
		// イテレーターを使用して敵弾の座標を取得する
		colliders_.push_back(it->get());
	}

	// 敵弾全てについて
	for (std::list<std::unique_ptr<EnemyBullet>>::const_iterator it = enemyBullets.begin();
	     it != enemyBullets.end(); ++it) {
		// イテレーターを使用して敵弾の座標を取得する
		colliders_.push_back(it->get());
	}

	// リスト内総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAから回す
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// ペアの当たり判定
			CheakCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheakCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
	    (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return;
	}
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();

	float distance;
	distance = Distance(posA, posB);
	// 球と球の当たり判定
	if (distance <= std::pow(colliderA->GetRadius() + colliderB->GetRadius(), 2)) {
		// 自キャラの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}