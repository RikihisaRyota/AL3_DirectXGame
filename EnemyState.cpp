#include "EnemyState.h"
#include "Enemy.h"

void Approach::Update(Enemy* pEnemy) {
	// キャラクターの移動速度
	const float kApproachSpeed = -0.2f;
	// 移動（ベクトルを加算）
	pEnemy->AddTanslation(Vector3(0.0f, 0.0f, kApproachSpeed));
	//if (pEnemy->GetTanslation().z < 0.0f) {
	//	pEnemy->changeState(new Leave);
	//	pEnemy->StateInitilize();
	//}
}

void Leave::Update(Enemy* pEnemy) {
	// キャラクターの移動速度
	const float kLeaveSpeed = -0.2f;
	// 移動（ベクトルを加算）
	pEnemy->AddTanslation(Vector3(kLeaveSpeed, 0.0f, kLeaveSpeed));
	
}
