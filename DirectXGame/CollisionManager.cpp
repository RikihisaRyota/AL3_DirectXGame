#include "CollisionManager.h"
#include "Vector3.h"
#include "MyMath.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"


void CollisionManager::Update(
    Player* player, 
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullet,
	const std::list<std::unique_ptr<Enemy>>& enemy,
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet) { 
	colliders_.clear();
	CheckAllCollisions(player, playerBullet, enemy, enemyBullet);
}


void CollisionManager::CheckAllCollisions(
    Player* player, 
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullet,
    const std::list<std::unique_ptr<Enemy>>& enemy_,
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet) {

	// 敵のリストに取得
	const std::list<std::unique_ptr<Enemy>>& enemy = enemy_;
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = playerBullet;
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullet;

	// コライダーをリストに追加
	colliders_.push_back(player);

	// 敵のリスト
	for (std::list<std::unique_ptr<Enemy>>::const_iterator it = enemy.begin(); it != enemy.end();++it) {
		// イテレーターを使用して敵弾の座標を取得する
		colliders_.push_back(it->get());
	}

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