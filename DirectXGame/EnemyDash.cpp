#include "EnemyDash.h"

#include "Enemy.h"
#include "EnemyAttack.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"

void EnemyDash::Initialize() { 
	t_ = 0.0f;
	kTarget_Speed_ = 0.02f;
	kDash_Speed_ = 0.005f;
	target_ = GetPlayer()->GetWorldTransform();
	// 落下地点をセット
	GetEnemyAttack()->SetWorldtransform(GetEnemy()->GetWorldTransform());
	WorldTransform worldTransform = GetEnemyAttack()->GetWorldTransform();
	// スケール
	const float kScale = 5.0f;
	// 角度
	Vector3 rotate = {0.0f,0.0f,0.0f};
	// 移動
	Vector3 transform = target_.translation_;
	// 代入
	worldTransform.scale_.z = kScale;
	worldTransform.rotation_ = rotate;
	worldTransform.translation_ += transform; 
	GetEnemyAttack()->SetWorldtransform(worldTransform);

	SetWorking(true);
	state_ = EnemyDash::State::kTarget;
}

void EnemyDash::Update() {
	switch (state_) {
	case EnemyDash::State::kRoot:
	default:
		RootUpdate();
		break;
	case EnemyDash::State::kTarget:
		TargetUpdate();
		break;
	case EnemyDash::State::kDash:
		DrapUpdate();
		break;
	}
}

void EnemyDash::RootUpdate() { 
	SetWorking(false);
}

void EnemyDash::TargetUpdate() {
	t_ += kTarget_Speed_;
	// スタートの角度
	Vector3 rotate_Start = GetEnemy()->GetWorldTransform().rotation_;
	if (rotate_Start != Vector3(0.0f, 0.0f, 0.0f)) {
		rotate_Start.Normalize();
	}
	// エンドの角度
	Vector3 rotate_End = GetPlayer()->GetWorldTransform().translation_ - GetEnemy()->GetWorldTransform().translation_;
	if (rotate_End != Vector3(0.0f, 0.0f, 0.0f)) {
		rotate_End.Normalize();
	}
	rotate_End.y = atan2(rotate_End.x, rotate_End.z);
	Vector3 rotate;
	rotate.y = Lerp(rotate_Start.y, rotate_End.y, t_);
	GetEnemy()->SetRotation(rotate);
	if (t_ >= 1.0f) {
		state_ = State::kDash;
		t_ = 0.0f;
	}
}

void EnemyDash::DrapUpdate() {
	t_ += kDash_Speed_;
	Vector3 move = Lerp(GetEnemy()->GetWorldTransform().translation_, target_.translation_, t_);
	GetEnemy()->SetTranslation(move);
	if (t_ >= 1.0f) {
		state_ = State::kRoot;
		t_ = 0.0f;
	}
}
