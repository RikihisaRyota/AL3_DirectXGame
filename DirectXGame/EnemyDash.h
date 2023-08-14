#pragma once

#include "EnemyAttakType.h"

class EnemyDash : public EnemyAttackType {
public:
	// プレス
	enum class State {
		kRoot,
		kTarget,
		kDash,
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
	float kDash_Speed_;
	WorldTransform target_;
	State state_;
};