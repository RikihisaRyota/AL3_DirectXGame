#include "PlayerAttack.h"

#include "Collision.h"
#include "Draw.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "MyMath.h"
#include "Player.h"

void PlayerAttack::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// 基底クラス
	BaseCharacter::Initialize(std::move(model));
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	HitBoxInitialize();
}

void PlayerAttack::Initialize() {
	if (behaviorRequest_) {
		// ふるまいを変更
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case PlayerAttack::Behavior::kRoot:
		default:
			break;
		case PlayerAttack::Behavior::kChargeAttack:
			ChageAttackInitialize();
			break;
		case PlayerAttack::Behavior::kTripleAttack:
			TripleAttackInitialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void PlayerAttack::Update() {

	switch (behavior_) {
	case PlayerAttack::Behavior::kRoot:
	default:
		break;
	case PlayerAttack::Behavior::kChargeAttack:
		ChageAttackUpdate();
		break;
	case PlayerAttack::Behavior::kTripleAttack:
		TripleAttackUpdate();
		break;
	}
	BaseCharacter::Update();
	HitBoxUpdate();
}

void PlayerAttack::Draw(const ViewProjection& viewProjection) {
	switch (behavior_) {
	case PlayerAttack::Behavior::kRoot:
	default:
		break;
	case PlayerAttack::Behavior::kChargeAttack:
		models_[static_cast<int>(Parts::WEAPON)]->Draw(
		    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)], viewProjection);
		break;
	case PlayerAttack::Behavior::kTripleAttack:

		break;
	}
}

void PlayerAttack::ChageAttackInitialize() {
	worldTransform_ = player_->GetWorldTransform();
	worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x = 0.0f;

	// 剣のデットゾーン
	slashMin_ = DegToRad(-45.0f);
	slashMax_ = DegToRad(90.0f);

	slash_Attack_Min_ = DegToRad(RadToDeg(slashMin_) + 135.0f);
	slash_Attack_Max_ = DegToRad(RadToDeg(slashMax_) + 270.0f);

	slash_Attack_Start_ = 0.0f;
	slash_ArmAngle_Start_ = DegToRad(slash_Attack_Start_ + 180.0f);
	// 攻撃の溜めモーションスピード
	charge_Speed_ = 0.01f;
	charge_T_ = 0.0f;
	// 溜めているかどうかのフラグ
	chargeFlag_ = true;
	// 降り下ろしモーション
	slash_Speed_ = 0.2f;
	slash_T_ = 0.0f;
	// 溜めてあと立てるフラグ
	slashFlag_ = false;
	// 攻撃硬直
	rigorFlag_ = false;
	rigor_Speed_ = 0.1f;
	rigor_T_ = 0.0f;
}

void PlayerAttack::ChageAttackUpdate() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	worldTransform_ = player_->GetWorldTransform();
	// チャージ中
	if (chargeFlag_) {
		if (Input::GetInstance()->PushKey(DIK_Q) ||
		    (Input::GetInstance()->GetJoystickState(0, joyState) &&
		     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X))) {
			charge_T_ += charge_Speed_;
			worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x =
			    Lerp(slash_Attack_Start_, slashMin_, Clamp(charge_T_, 0.0f, 1.0f));
			WorldTransform worldtramsform =
			    player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
			worldtramsform.rotation_.x =
			    Lerp(slash_ArmAngle_Start_, slash_Attack_Min_, Clamp(charge_T_, 0.0f, 1.0f));
			player_->SetWorldtransforms_Parts(
			    worldtramsform, static_cast<int>(Player::Parts::ARML));
			player_->SetWorldtransforms_Parts(
			    worldtramsform, static_cast<int>(Player::Parts::ARMR));
		} else {
			// チャージ終わり
			chargeFlag_ = false;
			slashFlag_ = true;
			charge_T_ = 0.0f;
			slash_Attack_Start_ =
			    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x;
			slash_ArmAngle_Start_ =
			    player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML))
			        .rotation_.x;
		}
	}
	// 降り始め
	if (slashFlag_) {
		slash_T_ += slash_Speed_;
		float rotate = Lerp(slash_Attack_Start_, slashMax_, Clamp(slash_T_, 0.0f, 1.0f));
		worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x = rotate;
		WorldTransform worldtramsform =
		    player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
		rotate = Lerp(slash_ArmAngle_Start_, slash_Attack_Max_, Clamp(slash_T_, 0.0f, 1.0f));
		worldtramsform.rotation_.x = rotate;
		player_->SetWorldtransforms_Parts(worldtramsform, static_cast<int>(Player::Parts::ARML));
		player_->SetWorldtransforms_Parts(worldtramsform, static_cast<int>(Player::Parts::ARMR));
		if (worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x >=
		    slashMax_ - 0.00005f) {
			slashFlag_ = false;
			slash_T_ = 0.0f;
			rigorFlag_ = true;
		}
	}
	// 攻撃硬直
	if (rigorFlag_) {
		rigor_T_ += rigor_Speed_;
		if (rigor_T_ >= 1.0f) {
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
			rigorFlag_ = false;
		}
	}
	ImGui::Begin("PlayerAttack");
	ImGui::SliderFloat3("AABB_min", &min_.x, -3.0f, 0.0f);
	ImGui::SliderFloat3("AABB_max", &max_.x, 0.0f, 6.0f);
	ImGui::SliderFloat3("OBB_size", &size_.x, 0.0f, 6.0f);
	ImGui::SliderFloat("Sphere_radius", &radius_, 0.0f, 3.0f);
	ImGui::End();
}

void PlayerAttack::TripleAttackInitialize() {
	worldTransform_ = player_->GetWorldTransform();
	firstFlag = true;
	first_T_ = 0.0f;
	first_Speed_ = 0.1f;
	armAngleStart_ = 0.0f;
	armAngleMax_ =-2.0f/* DegToRad(45.0f)*/;
	armSlideStart_ = 0.0f;
	armSlideMax_ = 0.5f;
	bodyAngleStart_ = 0.0f;
	bodyAngleMax_ = DegToRad(15.0f);
	
	second_T_=0.0f;
	second_Speed_ = 0.1f;

}

void PlayerAttack::TripleAttackUpdate() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	if (firstFlag) {
		first_T_ += first_Speed_;
		WorldTransform armWorldtramsform = player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARMR));
		WorldTransform motionWorldtramsform = player_->GetWorldTransform_Motion();
		WorldTransform worldtramsform = player_->GetWorldTransform();

		float armrotate = Lerp(armAngleStart_, armAngleMax_, Clamp(first_T_, 0.0f, 1.0f));
		float translation = Lerp(armSlideStart_, armSlideMax_, Clamp(first_T_, 0.0f, 1.0f));
		armWorldtramsform.rotation_.x = armrotate;
		armWorldtramsform.translation_.z = translation;
		float motionbodyrotate = Lerp(bodyAngleStart_, bodyAngleStart_, Clamp(first_T_, 0.0f, 1.0f));
		motionWorldtramsform.rotation_.y = motionbodyrotate;

		worldtramsform.translation_.z += armWorldtramsform.translation_.z;
		player_->SetWorldtransforms_Parts(armWorldtramsform, static_cast<int>(Player::Parts::ARMR));
		player_->SetWorldtransform_Motion(motionWorldtramsform);
		// 範囲内にはいていたらホーミング
		if (IsCollision(*enemy_->GetAABB(), aabb_)) {
			Vector3 toEnemy =
			    enemy_->GetWorldTransform().translation_ - worldTransform_.translation_;
			toEnemy.Normalize();
			worldTransform_.translation_ += Lerp(Vector3(0.0f, 0.0f, 0.0f), toEnemy, 0.2f);
			player_->SetTranslation(worldTransform_.translation_);
			player_->PlayerRotate(toEnemy);
		}
		if (first_T_ >= 1.0f) {
			if (Input::GetInstance()->TriggerKey(DIK_E) ||
			    (Input::GetInstance()->GetJoystickState(0, joyState) &&
			     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))) {
				secondFlag = true;
				firstFlag = false;
			}
		} 
		if (first_T_ >= 1.5f) {
			first_T_ = 0.0f;
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	if (secondFlag) {
		second_T_ += second_Speed_;
		WorldTransform armWorldtramsform =
		    player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
		WorldTransform motionWorldtramsform = player_->GetWorldTransform_Motion();
		WorldTransform worldtramsform = player_->GetWorldTransform();

		float armrotate = Lerp(armAngleStart_, armAngleMax_, Clamp(second_T_, 0.0f, 1.0f));
		float translation = Lerp(armSlideStart_, armSlideMax_, Clamp(second_T_, 0.0f, 1.0f));
		armWorldtramsform.rotation_.x = armrotate;
		armWorldtramsform.translation_.z = translation;
		float motionbodyrotate =
		    Lerp(bodyAngleStart_, bodyAngleStart_, Clamp(second_T_, 0.0f, 1.0f));
		motionWorldtramsform.rotation_.y = motionbodyrotate;

		worldtramsform.translation_.z += armWorldtramsform.translation_.z;
		player_->SetWorldtransforms_Parts(armWorldtramsform, static_cast<int>(Player::Parts::ARML));
		player_->SetWorldtransform_Motion(motionWorldtramsform);
		// 範囲内にはいていたらホーミング
		if (IsCollision(*enemy_->GetAABB(), aabb_)) {
			Vector3 toEnemy =
			    enemy_->GetWorldTransform().translation_ - worldTransform_.translation_;
			toEnemy.Normalize();
			worldTransform_.translation_ += Lerp(Vector3(0.0f, 0.0f, 0.0f), toEnemy, 0.2f);
			player_->SetTranslation(worldTransform_.translation_);
			player_->PlayerRotate(toEnemy);
		}
		if (first_T_ >= 1.0f) {
			if (Input::GetInstance()->TriggerKey(DIK_E) ||
			    (Input::GetInstance()->GetJoystickState(0, joyState) &&
			     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))) {
				secondFlag = true;
				firstFlag = false;
			}
		}
		if (first_T_ >= 1.5f) {
			first_T_ = 0.0f;
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
		}
	}
}

void PlayerAttack::HitBoxUpdate() {

	switch (behavior_) {
	case PlayerAttack::Behavior::kRoot:
	default:
		break;
	case PlayerAttack::Behavior::kChargeAttack:
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ + min_},
		    .max_{aabb_.center_ + max_},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{size_}
        };
		obb_ = OBBSetRotate(
		    obb_, worldTransform_.rotation_,
		    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_);
		break;
	case PlayerAttack::Behavior::kTripleAttack:
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ + tripleAttackMin_},
		    .max_{aabb_.center_ + tripleAttackMax_},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{tripleAttackSize_}
        };
		obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
		break;
	}

	// Sphere
	sphere_ = {
	    .center_{worldTransform_.translation_},
	    .radius_{radius_},
	};
}

void PlayerAttack::HitBoxInitialize() {
	// AABB
	min_ = {-5.1f, -0.9f, -5.1f};
	max_ = {5.1f, 5.1f, 5.1f};
	tripleAttackMin_ = {-3.0f, -0.9f, -3.0f};
	tripleAttackMax_ = {3.0f, 1.0f, 3.0f};
	// OBB
	size_ = {0.5f, 5.1f, 0.5f};
	tripleAttackSize_ = {0.5f, 0.5f, 0.5f};
	// Sphere
	radius_ = 1.2f;
	switch (behavior_) {
	case PlayerAttack::Behavior::kRoot:
	default:
		break;
	case PlayerAttack::Behavior::kChargeAttack:
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ + min_},
		    .max_{aabb_.center_ + max_},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{size_}
        };
		obb_ = OBBSetRotate(
		    obb_, worldTransform_.rotation_,
		    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_);
		break;
	case PlayerAttack::Behavior::kTripleAttack:
		// AABB
		aabb_ = {
		    .center_{worldTransform_.translation_},
		    .min_{aabb_.center_ + tripleAttackMin_},
		    .max_{aabb_.center_ + tripleAttackMax_},
		};
		// OBB
		obb_ = {
		    .center_{worldTransform_.translation_},
		    .orientations_{
		             {1.0f, 0.0f, 0.0f},
		             {0.0f, 1.0f, 0.0f},
		             {0.0f, 0.0f, 1.0f},
		             },
		    .size_{tripleAttackSize_}
        };
		obb_ = OBBSetRotate(obb_, worldTransform_.rotation_);
		break;
	}
}
void PlayerAttack::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawAABB(aabb_, viewProjection, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawOBB(obb_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void PlayerAttack::OnCollision(const OBB& obb) { OBB a = obb; }
