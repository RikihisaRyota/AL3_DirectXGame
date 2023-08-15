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
	armAngleMax_ = DegToRad(90.0f);
	armSlideStart_ = 0.0f;
	armSlideMax_ = 0.5f;
	bodyAngleStart_ = 0.0f;
	bodyAngleMax_ = DegToRad(15.0f);

	secondFlag = false;
	second_T_ = 0.0f;
	second_Speed_ = 0.1f;

	thirdFlag = false;
	third_T_ = 0.0f;
	third_Speed_ = 0.1f;

}

void PlayerAttack::TripleAttackUpdate() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	if (firstFlag) {
		first_T_ += first_Speed_;
		WorldTransform armWorldtramsform = player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARMR));
		WorldTransform motionWorldtramsform = player_->GetWorldTransform_Motion();

		float armrotate = Lerp(armAngleStart_, armAngleMax_, Clamp(first_T_, 0.0f, 1.0f));
		float translation = Lerp(armSlideStart_, armSlideMax_, Clamp(first_T_, 0.0f, 1.0f));
		armWorldtramsform.rotation_.x = armrotate;
		armWorldtramsform.translation_.z = translation;
		player_->SetWorldtransforms_Parts(armWorldtramsform, static_cast<int>(Player::Parts::ARMR));

		float motionbodyrotate = Lerp(bodyAngleStart_,-bodyAngleMax_, Clamp(first_T_, 0.0f, 1.0f));
		motionWorldtramsform.rotation_.y = motionbodyrotate;
		player_->SetWorldtransform_Motion(motionWorldtramsform);

		// 回転行列生成
		Matrix4x4 rotate = MakeRotateYMatrix(player_->GetWorldTransform().rotation_.y);
		// オフセットをカメラの回転に合わせて回転させる
		center_ = TransformNormal(center_Distance_, rotate);
		// ホーミング
		if (first_T_ <= 1.0f) {
			Homing();
		}
	
		if (first_T_ >= 1.0f) {
			if (Input::GetInstance()->TriggerKey(DIK_E) ||
			    (Input::GetInstance()->GetJoystickState(0, joyState) &&
			     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))) {
				secondFlag = true;
				firstFlag = false;
				armWorldtramsform.translation_ = Vector3(0.0f, 0.0f, 0.0f);
				armWorldtramsform.rotation_= Vector3(0.0f, 0.0f, 0.0f);
				player_->SetWorldtransforms_Parts(armWorldtramsform, static_cast<int>(Player::Parts::ARMR));
			}
		} 
		if (first_T_ >= 2.0f) {
			firstFlag = false;
			first_T_ = 0.0f;
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
		}
	} else if (secondFlag) {
		second_T_ += second_Speed_;
		WorldTransform armWorldtramsform =
		    player_->GetWorldTransforms_Parts(static_cast<int>(Player::Parts::ARML));
		WorldTransform motionWorldtramsform = player_->GetWorldTransform_Motion();

		float armrotate = Lerp(armAngleStart_, armAngleMax_, Clamp(second_T_, 0.0f, 1.0f));
		float translation = Lerp(armSlideStart_, armSlideMax_, Clamp(second_T_, 0.0f, 1.0f));
		armWorldtramsform.rotation_.x = armrotate;
		armWorldtramsform.translation_.z = translation;
		player_->SetWorldtransforms_Parts(armWorldtramsform, static_cast<int>(Player::Parts::ARML));

		float motionbodyrotate = Lerp(bodyAngleStart_, bodyAngleMax_, Clamp(second_T_, 0.0f, 1.0f));
		motionWorldtramsform.rotation_.y = motionbodyrotate;
		player_->SetWorldtransform_Motion(motionWorldtramsform);

		// 回転行列生成
		Matrix4x4 rotate = MakeRotateYMatrix(player_->GetWorldTransform().rotation_.y);
		// 回転に合わせて回転させる
		center_ = TransformNormal(center_Distance_, rotate);
		// ホーミング
		if (second_T_ <= 1.0f) {
			Homing();
		}
		if (second_T_ >= 1.0f) {
			if (Input::GetInstance()->TriggerKey(DIK_E) ||
			    (Input::GetInstance()->GetJoystickState(0, joyState) &&
			     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))) {
				secondFlag = false;
				thirdFlag = true;
				armWorldtramsform.translation_ = Vector3(0.0f, 0.0f, 0.0f);
				armWorldtramsform.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
				player_->SetWorldtransforms_Parts(
				    armWorldtramsform, static_cast<int>(Player::Parts::ARML));
				motionWorldtramsform.rotation_ = Vector3(0.0f, 0.0f, 0.0f);
				player_->SetWorldtransform_Motion(motionWorldtramsform);
			}
		}
		if (second_T_ >= 1.5f) {
			secondFlag = false;
			second_T_ = 0.0f;
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
		}
	} else if (thirdFlag) {
		third_T_ += third_Speed_;
		WorldTransform motionWorldtramsform = player_->GetWorldTransform_Motion();
		float bodyrotate = Lerp(0.0f, DegToRad(720.0f), Clamp(third_T_, 0.0f, 1.0f));
		motionWorldtramsform.rotation_.x = bodyrotate;
		player_->SetWorldtransform_Motion(motionWorldtramsform);
		// 回転行列生成
		Matrix4x4 rotate = MakeRotateYMatrix(player_->GetWorldTransform().rotation_.y);
		// 回転に合わせて回転させる
		center_ = TransformNormal(center_Distance_, rotate);
		// ホーミング
		if (third_T_ <= 1.0f) {
			Homing();
		}
		if (third_T_ >= 2.5f) {
			thirdFlag = false;
			third_T_ = 0.0f;
			player_->SetBehavior(Player::Behavior::kRoot);
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	ImGui::Begin("PlayerAttack");
	ImGui::SliderFloat3("AABB_min", &tripleAttackMin_.x, -3.0f, 0.0f);
	ImGui::SliderFloat3("AABB_max", &tripleAttackMax_.x, 0.0f, 6.0f);
	ImGui::SliderFloat3("OBB_size", &tripleAttackSize_.x, 0.0f, 6.0f);
	ImGui::SliderFloat3("center_Distance_", &center_Distance_.x, 0.0f, 3.0f);
	ImGui::End();
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
		    .center_{worldTransform_.translation_ + center_},
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

void PlayerAttack::Homing() {
	// 範囲内にはいていたらホーミング
	if (IsCollision(*enemy_->GetAABB(), aabb_)) {
		Vector3 toEnemy = enemy_->GetWorldTransform().translation_ - worldTransform_.translation_;
		// 長さが1.0f以上ならホーミング
		const float kLength = player_->GetOBB()->size_.x + enemy_->GetOBB()->size_.x;
		float distance = toEnemy.Length(); 
		if (distance >= kLength) {
			toEnemy.Normalize();
			worldTransform_.translation_ += Lerp(Vector3(0.0f, 0.0f, 0.0f), toEnemy, 0.4f);
			worldTransform_.translation_.y = 1.0f;
			player_->SetTranslation(worldTransform_.translation_);
			player_->PlayerRotate(toEnemy);
		} else {
			// それ以下ならプレイヤーの回転だけセットする
			toEnemy.Normalize();
			player_->PlayerRotate(toEnemy);
		}
		
	}
}

void PlayerAttack::HitBoxInitialize() {
	// AABB
	min_ = {-5.1f, -0.9f, -5.1f};
	max_ = {5.1f, 5.1f, 5.1f};
	tripleAttackMin_ = {-4.0f, -0.9f, -4.0f};
	tripleAttackMax_ = {4.0f, 1.0f, 4.0f};
	// OBB
	size_ = {1.0f, 5.1f, 0.5f};
	tripleAttackSize_ = {1.0f, 1.0f, 1.0f};

	center_Distance_ = {0.0f, 0.0f, 1.0f};
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
