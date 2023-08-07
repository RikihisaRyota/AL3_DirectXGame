#include "PlayerAttack.h"

#include "Draw.h"
#include "Input.h"
#include "ImGuiManager.h"
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

void PlayerAttack::Update() {
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
			WorldTransform worldtramsform = player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
			worldtramsform.rotation_.x = Lerp(slash_ArmAngle_Start_, slash_Attack_Min_, Clamp(charge_T_, 0.0f, 1.0f));
			player_->SetWorldtransforms_Parts(worldtramsform, static_cast<int>(Player::Parts::ARML));
			player_->SetWorldtransforms_Parts(worldtramsform, static_cast<int>(Player::Parts::ARMR));
		} else {
			// チャージ終わり
			chargeFlag_ = false;
			slashFlag_ = true;
			charge_T_ = 0.0f;
			slash_Attack_Start_ = worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x;
			slash_ArmAngle_Start_ = player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML)).rotation_.x;
		}
	}
	// 降り始め
	if (slashFlag_) {
		slash_T_ += slash_Speed_;
		float rotate = Lerp(slash_Attack_Start_, slashMax_, Clamp(slash_T_, 0.0f, 1.0f));
		worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x = rotate;
		WorldTransform worldtramsform = player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
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
			rigorFlag_ = false;
		}
	}
	ImGui::Begin("PlayerAttack");
	ImGui::SliderFloat3("AABB_min", &min_.x, -3.0f, 0.0f);
	ImGui::SliderFloat3("AABB_max", &max_.x, 0.0f, 6.0f);
	ImGui::SliderFloat3("OBB_size", &size_.x, 0.0f, 6.0f);
	ImGui::SliderFloat("Sphere_radius", &radius_, 0.0f, 3.0f);
	ImGui::End();
	BaseCharacter::Update();
	HitBoxUpdate();
}

void PlayerAttack::Draw(const ViewProjection& viewProjection) {
	models_[static_cast<int>(Parts::WEAPON)]->Draw(
	    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)], viewProjection);
}

void PlayerAttack::HitBoxUpdate() {
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
		obb_, 
		worldTransform_.rotation_,
		worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_);

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
	// OBB
	size_ = {0.5f, 5.1f, 0.5f};
	// Sphere
	radius_ = 1.2f;
	// AABB
	aabb_ = {
	    .center_{worldTransform_.translation_},
	    .min_{aabb_.center_ + min_},
	    .max_{aabb_.center_ + max_},
	};
	// OBB
	obb_ = {
	    .center_{
	             worldTransform_.translation_.x,
				 worldTransform_.translation_.y+3.0f,
	             worldTransform_.translation_.z},
	    .orientations_{
	             {1.0f,0.0f,0.0f},
	             {0.0f,1.0f,0.0f},
	             {0.0f,0.0f,1.0f},
	             },
	    .size_{size_}
    };
	obb_ = OBBSetRotate(
	    obb_, worldTransform_.rotation_,
	    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_);
}
void PlayerAttack::HitBoxDraw(const ViewProjection& viewProjection) {
	DrawAABB(aabb_, viewProjection, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	DrawOBB(obb_, viewProjection, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
}

void PlayerAttack::OnCollision(const OBB& obb) { OBB a = obb; }
