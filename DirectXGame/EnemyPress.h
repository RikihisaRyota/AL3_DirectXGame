#pragma once

#include "EnemyAttakType.h"

class EnemyPress : public EnemyAttackType {
public:
	// プレス
	enum class State {
		kRoot,
		kTarget,
		kDrap,
	};
public:
	void Initialize() override;
	void Update() override;
private:
	void RootUpdate();
	void TargetUpdate();
	void DrapUpdate();

private:
	float t_;
	float kTarget_Speed_;
	float kDrop_Speed_;
	WorldTransform target_;
	State state_;
};
