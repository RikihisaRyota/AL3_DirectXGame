#pragma once

#include "WorldTransform.h"

class Enemy;
class Player;
class EnemyAttack;
class EnemyAttackType {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void SetPlayerEnemy(Player* player, Enemy* enemy, EnemyAttack* enemyAttack) {
		player_ = player, enemy_ = enemy, enemyAttack_=enemyAttack;
	}
	virtual bool GetWorking() { return IsWorking_; }
	virtual void SetWorking(bool working) { IsWorking_ = working; }
	virtual Player* GetPlayer() { return player_; }
	virtual Enemy* GetEnemy() { return enemy_; }
	virtual EnemyAttack* GetEnemyAttack() { return enemyAttack_; }

private:
	bool IsWorking_ = false;
	Enemy* enemy_;
	Player* player_;
	EnemyAttack* enemyAttack_;
};