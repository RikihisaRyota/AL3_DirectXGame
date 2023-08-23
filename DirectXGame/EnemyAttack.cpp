#include "EnemyAttack.h"

#include "Collision.h"
#include "Draw.h"
#include "Enemy.h"
#include "Input.h"
#include "MyMath.h"
#include "Player.h"
#include "PlayerHP.h"

void EnemyAttack::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// 基底クラス
	BaseCharacter::Initialize(std::move(model));
	worldTransform_.scale_ = {4.0f, 1.0f, 4.0f};
	worldTransform_.UpdateMatrix();
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyAttack);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemyAttack);
	HitBoxInitialize();

	press_ = std::make_unique<EnemyPress>();
	press_->SetPlayerEnemy(player_, enemy_, this);

	dash_ = std::make_unique<EnemyDash>();
	dash_->SetPlayerEnemy(player_, enemy_, this);

	punch_ = std::make_unique<EnemyPunch>();
	punch_->SetPlayerEnemy(player_, enemy_, this);
}

void EnemyAttack::Initialize() {
	if (behaviorRequest_) {
		// ふるまいを変更
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case EnemyAttack::Behavior::kRoot:
		default:
			RootInitialize();
			break;
		case EnemyAttack::Behavior::kPressAttack:
			press_->Initialize();
			break;
		case EnemyAttack::Behavior::kDashAttack:
			dash_->Initialize();
			break;
		case EnemyAttack::Behavior::kPunchAttack:
			punch_->Initialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void EnemyAttack::RootInitialize() {
	worldTransform_.scale_ = {4.0f, 1.0f, 4.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.UpdateMatrix();
}

void EnemyAttack::Update() {
	if (enemy_->GetBehavior() == Enemy::Behavior::kAttack) {
		if (behaviorRequest_) {
			// ふるまいを変更
			behavior_ = behaviorRequest_.value();
			// 各ふるまいごとの初期化を実行
			switch (behavior_) {
			case EnemyAttack::Behavior::kRoot:
			default:
				RootInitialize();
				break;
			case EnemyAttack::Behavior::kPressAttack:
				press_->Initialize();
				break;
			case EnemyAttack::Behavior::kDashAttack:
				dash_->Initialize();
				break;
			case EnemyAttack::Behavior::kPunchAttack:
				punch_->Initialize();
				break;
			}
			// ふるまいリクエストをリセット
			behaviorRequest_ = std::nullopt;
		}
		switch (behavior_) {
		case EnemyAttack::Behavior::kRoot:
		default:
			enemy_->SetBehavior(Enemy::Behavior::kRoot);
			break;
		case EnemyAttack::Behavior::kPressAttack:
			press_->Update();
			if (!press_->GetWorking()) {
				behaviorRequest_ = EnemyAttack::Behavior::kRoot;
			}
			break;
		case EnemyAttack::Behavior::kDashAttack:
			dash_->Update();
			if (!dash_->GetWorking()) {
				behaviorRequest_ = EnemyAttack::Behavior::kRoot;
			}
			break;
		case EnemyAttack::Behavior::kPunchAttack:
			punch_->Update();
			if (!punch_->GetWorking()) {
				behaviorRequest_ = EnemyAttack::Behavior::kRoot;
			}
			break;
		}
		BaseCharacter::Update();
		HitBoxUpdate();
	}
}

void EnemyAttack::Draw(const ViewProjection& viewProjection) {
	switch (behavior_) {
	case EnemyAttack::Behavior::kRoot:
	default:
		break;
	case EnemyAttack::Behavior::kPressAttack:
		models_[static_cast<int>(EnemyAttack::Parts::CIRCLE)]->Draw(
		    worldTransforms_Parts_[static_cast<int>(EnemyAttack::Parts::CIRCLE)], viewProjection);
		break;
	case EnemyAttack::Behavior::kDashAttack:
		models_[static_cast<int>(EnemyAttack::Parts::PLANE)]->Draw(
		    worldTransforms_Parts_[static_cast<int>(EnemyAttack::Parts::PLANE)], viewProjection);
		break;
	case EnemyAttack::Behavior::kPunchAttack:
		models_[static_cast<int>(EnemyAttack::Parts::CIRCLE)]->Draw(
		    worldTransforms_Parts_[static_cast<int>(EnemyAttack::Parts::CIRCLE)], viewProjection);
		break;
	}
}

void EnemyAttack::HitBoxInitialize() {
	// AABB
	min_ = {-5.0f, -0.9f, -5.0f};
	max_ = {5.0f, 1.0f, 5.0f};
	// OBB
	size_ = {4.0f, 1.0f, 4.0f};
	// Sphere
	radius_ = 1.2f;
	// AABB
	aabb_ = {
	    .center_{worldTransform_.translation_},
	    .min_{aabb_.center_ - worldTransform_.scale_},
	    .max_{aabb_.center_ + worldTransform_.scale_},
	};
	// OBB
	obb_ = {
	    .center_{worldTransform_.translation_},
	    .orientations_{
	             {1.0f, 0.0f, 0.0f},
	             {0.0f, 1.0f, 0.0f},
	             {0.0f, 0.0f, 1.0f},
	             },
	    .size_{worldTransform_.scale_}
    };
	obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
	// Sphere
	sphere_ = {
	    .center_{worldTransform_.translation_},
	    .radius_{radius_},
	};
}

void EnemyAttack::HitBoxUpdate() {
	switch (behavior_) {
	case EnemyAttack::Behavior::kRoot:
	default:
		break;
	case EnemyAttack::Behavior::kPressAttack: 
	{
		float size = (std::max)(
		    (std::max)(worldTransform_.scale_.x, worldTransform_.scale_.y),
		    worldTransform_.scale_.z);
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ - size},
		    .max_{aabb_.center_ + size},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{worldTransform_.scale_}
        };
		obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
		break;
	}
	case EnemyAttack::Behavior::kDashAttack: {
		float size = (std::max)(
		    (std::max)(worldTransform_.scale_.x, worldTransform_.scale_.y),
		    worldTransform_.scale_.z);
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ - size},
		    .max_{aabb_.center_ + size},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{worldTransform_.scale_}
        };
		obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
		break;
	}
	case EnemyAttack::Behavior::kPunchAttack: {
		float size = (std::max)(
		    (std::max)(worldTransform_.scale_.x, worldTransform_.scale_.y),
		    worldTransform_.scale_.z);
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ - size},
		    .max_{aabb_.center_ + size},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{worldTransform_.scale_}
        };
		obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
		break;
	}
	}
}

void EnemyAttack::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawAABB(aabb_, viewProjection, Vector4(0.0f, 0.5f, 0.25f, 1.0f));
	DrawOBB(obb_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void EnemyAttack::OnCollision(const OBB& obb, uint32_t type) {
	OBB obb1 = obb;
	uint32_t i = type;
	i;
	switch (behavior_) {
	case EnemyAttack::Behavior::kRoot:
	default:
		break;
	case EnemyAttack::Behavior::kPressAttack:
		if (press_->GetAttack()) {
			if (IsCollision(
			    OBB(*player_->GetOBB()), Sphere(GetOBB()->center_, GetOBB()->size_.z))) {
				PlayerHP::SetAdd(70);
				press_->SetHit(true);
			}
		}
		break;
	case EnemyAttack::Behavior::kDashAttack:
		if (dash_->GetAttack()) {
			if (IsCollision(*player_->GetOBB(), *enemy_->GetOBB())) {
				PlayerHP::SetAdd(40);
				dash_->SetHit(true);
			}
		}
		break;
	case EnemyAttack::Behavior::kPunchAttack:
		if (punch_->GetAttack()) {
			PlayerHP::SetAdd(30);
			punch_->SetHit(true);
		}
		break;
	}
}