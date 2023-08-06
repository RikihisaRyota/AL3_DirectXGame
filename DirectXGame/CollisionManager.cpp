#include "CollisionManager.h"

#include "Collision.h"
#include "MyMath.h"

#include "ImGuiManager.h"

void CollisionManager::Update(Player* player, Enemy* enemy) {
	colliders_.clear();
	CheckAllCollisions(player, enemy);
}

void CollisionManager::CheckAllCollisions(Player* player, Enemy* enemy) {
	// プレイヤーをリストに追加
	colliders_.emplace_back(player);
	// 敵をリストに追加
	colliders_.emplace_back(enemy);

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
	AABB* aabbA = colliderA->GetAABB();
	AABB* aabbB = colliderB->GetAABB();
	ImGui::Begin("AABB");
	ImGui::Text("AABB_center x:%f,y:%f,z:%f", aabbA->center_.x, aabbA->center_.y, aabbA->center_.z);
	ImGui::Text("AABB_min x:%f,y:%f,z:%f", aabbA->min_.x, aabbA->min_.y, aabbA->min_.z);
	ImGui::Text("AABB_max x:%f,y:%f,z:%f", aabbA->max_.x, aabbA->max_.y, aabbA->max_.z);
	ImGui::Text("AABBB_center x:%f,y:%f,z:%f", aabbB->center_.x, aabbB->center_.y, aabbB->center_.z);
	ImGui::Text("AABBB_mix x:%f,y:%f,z:%f", aabbB->min_.x, aabbB->min_.y, aabbB->min_.z);
	ImGui::Text("AABBB_max x:%f,y:%f,z:%f", aabbB->max_.x, aabbB->max_.y, aabbB->max_.z);
	ImGui::End();
	if (IsCollision(*aabbA, *aabbB)) {
		// 自キャラの衝突時コールバックを呼び出す
		colliderA->OnCollision(*aabbB);
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision(*aabbA);
	}
}
