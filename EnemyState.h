#pragma once
class Enemy;
class EnemyState {
public:
	/*virtual ~EnemyState() = 0;*/
	virtual void Update(Enemy* pEnemy) = 0;
};

class Approach : public EnemyState {
public:
	void Update(Enemy* pEnemy);
};

class Leave : public EnemyState {
public:
	void Update(Enemy* pEnemy);
};
