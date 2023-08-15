#pragma once

#include "EnemyAttakType.h"

class EnemyPress : public EnemyAttackType {
public:
	// プレス
	enum class State {
		kRoot,
		kTarget,
		kStay,
		kDrap,
	};
public:
	void Initialize() override;
	void Update() override;
private:
	void RootUpdate();
	void StayUpdate();
	void TargetUpdate();
	void DrapUpdate();

private:
	float t_;
	float kTarget_Speed_;
	float kStay_Speed_;
	float kDrop_Speed_;
	WorldTransform target_;
	WorldTransform origin_;
	State state_;
};
