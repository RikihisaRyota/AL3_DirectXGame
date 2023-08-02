#include "Player.h"

#include <cassert>

#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "MyMath.h"

void Player::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// 基底クラス
	BaseCharacter::Initialize(std::move(model));
	//SetGlobalVariables();
	GetGlobalVariables();
	// 方向
	interRotate_ = {0.0f, 0.0f, 1.0f};
	// ジャンプフラグ
	isJump = false;
	// 浮遊アニメーションの初期化
	InitializeFloatGimmick();

}

void Player::Update() {
	if (behaviorRequest_) {
		// ふるまいを変更
		behavior_ = behaviorRequest_.value();
		// 各ふるまいごとの初期化を実行
		switch (behavior_) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Player::Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Player::Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Player::Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}
	
	// 転送
	BaseCharacter::Update();
//#ifdef DEBUG
	ImGui::Begin("Player");
	ImGui::Text(
	    "translation_ x:%f,y:%f,z:%f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text(
	    "rotate_ x:%f,y:%f,z:%f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z);
	ImGui::Text("interRotate_:%f,y:%f,z:%f", interRotate_.x, interRotate_.y, interRotate_.z);
	ImGui::Text(
	    "destinationAngle_:%f,y:%f,z:%f", destinationAngle_.x, destinationAngle_.y,
	    destinationAngle_.z);
	ImGui::Text("vector_x:%f,y:%f,z:%f", vector_.x, vector_.y, vector_.z);
	ImGui::Text("velocity_:%f,y:%f,z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("acceleration_:%f,y:%f,z:%f", acceleration_.x, acceleration_.y, acceleration_.z);
	ImGui::Text(
	    "worldTransforms_Parts_:%f",
	    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x);
	ImGui::Text("slashMax_:%f", slashMax_);
	ImGui::End();
//#endif // DEBUG
}

void Player::BehaviorRootUpdate() {
	// 入力
	GamePadInput();
	// プレイヤーモーションの更新
	Motion();
	if (isJump) {
		if (acceleration_.y > 0.0f) {
			// ジャンプ制作途中
		}
	}
}

void Player::BehaviorAttackInitialize() {
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

void Player::BehaviorAttackUpdate() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
#pragma region 攻撃
	// チャージ中
	if (chargeFlag_) {
		if (Input::GetInstance()->PushKey(DIK_Q) ||
			 (Input::GetInstance()->GetJoystickState(0, joyState) &&
	     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X))) {
			charge_T_ += charge_Speed_;
			worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x =
			    Lerp(slash_Attack_Start_, slashMin_, Clamp(charge_T_, 0.0f, 1.0f));
			worldTransforms_Parts_[static_cast<int>(Parts::ARML)].rotation_.x =
			    Lerp(slash_ArmAngle_Start_, slash_Attack_Min_, Clamp(charge_T_, 0.0f, 1.0f));
			worldTransforms_Parts_[static_cast<int>(Parts::ARMR)].rotation_.x =
			    Lerp(slash_ArmAngle_Start_, slash_Attack_Min_, Clamp(charge_T_, 0.0f, 1.0f));
		} else {
			// チャージ終わり
			chargeFlag_ = false;
			slashFlag_ = true;
			charge_T_ = 0.0f;
			slash_Attack_Start_ =
			    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x;
			slash_ArmAngle_Start_ =
			    worldTransforms_Parts_[static_cast<int>(Parts::ARML)].rotation_.x;
		}
	}
	// 降り始め
	if (slashFlag_) {
		slash_T_ += slash_Speed_;
		worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x =
		    Lerp(slash_Attack_Start_, slashMax_, Clamp(slash_T_, 0.0f, 1.0f));
		worldTransforms_Parts_[static_cast<int>(Parts::ARML)].rotation_.x =
		    Lerp(slash_ArmAngle_Start_, slash_Attack_Max_, Clamp(slash_T_, 0.0f, 1.0f));
		worldTransforms_Parts_[static_cast<int>(Parts::ARMR)].rotation_.x =
		    Lerp(slash_ArmAngle_Start_, slash_Attack_Max_, Clamp(slash_T_, 0.0f, 1.0f));
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
			behaviorRequest_ = Behavior::kRoot;
		}
	}
#pragma endregion
#pragma region 腕

#pragma endregion
#ifdef DEBUG
	ImGui::Begin("weapon");
	ImGui::SliderFloat("slashMin:%f", &slashMin_, 0.0f, -1.0f);
	ImGui::SliderFloat("slashMax:%f", &slashMax_, 1.5f, 0.0f);
	ImGui::SliderFloat("chargeSpeed:%f", &charge_Speed_, 0.1f, 0.01f);
	ImGui::SliderFloat("slashSpeed:%f", &slash_Speed_, 2.0f, 0.5f);
	ImGui::Text("rotate:%f", worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)].rotation_.x);
	ImGui::End();
#endif // DEBUG
}

void Player::BehaviorDashInitialize() { 
	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = std::atan2(destinationAngle_.x, destinationAngle_.z);
	acceleration_.y = 0.0f;
}

void Player::BehaviorDashUpdate() {
	
	// 自キャラの向いている方向に移動する処理
	const float kDashSpeed = 0.3f;
	velocity_ = destinationAngle_ * kDashSpeed;
	worldTransform_.translation_ += velocity_;

	// ダッシュの時間<frame>
	const uint32_t behaviorDashTime = 15;

	// 基底の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}

	worldTransform_Motion_.rotation_.x = Lerp(
	    DegToRad(0.0f), DegToRad(720.0f),
	    static_cast<float>(workDash_.dashParameter_) / static_cast<float>(behaviorDashTime));
	if (workDash_.dashParameter_ < behaviorDashTime * 0.5f) {
		worldTransform_Motion_.translation_.y = Lerp(
		    0, 0.5f,
		    static_cast<float>(workDash_.dashParameter_) / static_cast<float>(behaviorDashTime));
	} else {
		worldTransform_Motion_.translation_.y = Lerp(
		    0.5f, 0.0f,
		    static_cast<float>(workDash_.dashParameter_) / static_cast<float>(behaviorDashTime));
	}
	
}

void Player::SetGlobalVariables() {
	GlobalVariables* globalVariables = nullptr;
	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループ追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->SaveFile(groupName);
}

void Player::GetGlobalVariables() {
	GlobalVariables* globalVariables = nullptr;
	globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransform_.translation_ = globalVariables->GetValue<Vector3>(groupName, "position");
	// 転送
	BaseCharacter::Update();
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[static_cast<int>(Parts::HEAD)]->Draw(
	    worldTransforms_Parts_[static_cast<int>(Parts::HEAD)], viewProjection);
	models_[static_cast<int>(Parts::BODY)]->Draw(
	    worldTransforms_Parts_[static_cast<int>(Parts::BODY)], viewProjection);
	models_[static_cast<int>(Parts::ARML)]->Draw(
	    worldTransforms_Parts_[static_cast<int>(Parts::ARML)], viewProjection);
	models_[static_cast<int>(Parts::ARMR)]->Draw(
	    worldTransforms_Parts_[static_cast<int>(Parts::ARMR)], viewProjection);
	if (behavior_ == Behavior::kAttack) {
		models_[static_cast<int>(Parts::WEAPON)]->Draw(
		    worldTransforms_Parts_[static_cast<int>(Parts::WEAPON)], viewProjection);
	}
}

void Player::GamePadInput() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// プレイヤー移動
	Move();
	// 攻撃開始
	if (Input::GetInstance()->TriggerKey(DIK_Q)||
	    (Input::GetInstance()->GetJoystickState(0, joyState) &&
	     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X))
		) {
		behaviorRequest_ = Behavior::kAttack;
	}
	// ダッシュ開始
	if (Input::GetInstance()->TriggerKey(DIK_E) ||
	    (Input::GetInstance()->GetJoystickState(0, joyState) &&
		(joyState.Gamepad.bRightTrigger)) &&
	    (Input::GetInstance()->GetJoystickStatePrevious(0, joyState) &&
	    (!joyState.Gamepad.bRightTrigger))) {
		behaviorRequest_ = Behavior::kDash;
	}
	// ジャンプ
	Jump();
	// 重力
	Gravity();
}

void Player::Move() {
	// 移動量
	vector_ = {0.0f, 0.0f, 0.0f};
#pragma region ゲームパット
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// ゲームパットの状況取得
	// 入力がなかったら何もしない
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float kMargin = 0.7f;
		// 移動量
		Vector3 move = {
		    static_cast<float>(joyState.Gamepad.sThumbLX),
		    0.0f,
		    static_cast<float>(joyState.Gamepad.sThumbLY),
		};
		if (move.Length() > kMargin) {
			vector_ = {
			    static_cast<float>(joyState.Gamepad.sThumbLX),
			    0.0f,
			    static_cast<float>(joyState.Gamepad.sThumbLY),
			};
		}
	}
#pragma endregion
#pragma region キーボード
	if (Input::GetInstance()->PushKey(DIK_W)) {
		vector_.z = 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		vector_.z = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		vector_.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		vector_.x = 1.0f;
	}
	// 移動量に速さを反映
	if (vector_ != Vector3(0.0f, 0.0f, 0.0f)) {
		vector_.Normalize();
	}
#pragma endregion
#pragma region 正規化と回転
	// 移動量に速さを反映
	if (vector_ != Vector3(0.0f, 0.0f, 0.0f)) {
		vector_.Normalize();
		// 回転行列生成
		Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_->rotation_.y);
		// オフセットをカメラの回転に合わせて回転させる
		vector_ = TransformNormal(vector_, rotate);
		destinationAngle_ = vector_;
	}
#pragma endregion
}

void Player::Jump() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// ゲームパットの状況取得
	// 入力がなかったら何もしない
	if (((Input::GetInstance()->GetJoystickState(0, joyState) &&
	     (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) ||
	    Input::GetInstance()->TriggerKey(DIK_SPACE)) && !isJump) {
		acceleration_.y = kJumpPower;
		isJump = true;
	}
}
void Player::Gravity() {
	velocity_ = vector_ * kSpeed;
	velocity_ += acceleration_;
	worldTransform_.translation_ += velocity_;

	if (std::fabs(velocity_.x) <= 0.001 && std::fabs(velocity_.z) <= 0.001) {
		velocity_.x = 0.0f;
		velocity_.z = 0.0f;
	}
	if (worldTransform_.translation_.y > kGroundDistanse) {
		acceleration_.y -= kGravity;
	} else {
		acceleration_.y = 0.0f;
		worldTransform_.translation_.y = kGroundDistanse;
		isJump = false;
	}
}
void Player::PlayerRotate() {
	if (vector_ != Vector3(0.0f, 0.0f, 0.0f)) {
		vector_.Normalize();
	}
	if (interRotate_ != Vector3(0.0f, 0.0f, 0.0f)) {
		interRotate_.Normalize();
	}
	Vector3 rotate = Lerp(interRotate_, vector_, kTurn);
	//  Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(rotate.x, rotate.z);
	// プレイヤーの向いている方向
	interRotate_ = rotate;
}
void Player::InitializeFloatGimmick() { 
	floatingParameter_ = 0.0f;
	worldTransform_Motion_.rotation_ = {0.0f, 0.0f, 0.0f};
}
void Player::Motion() {
	// 全体
	Base();
	// 頭
	Head();
	// 体
	Body();
	// 左腕
	ArmLeft();
	// 右腕
	ArmRight();
	// 転送
	// UpdateMotionMatrix();
}
void Player::UpdateMotionMatrix() {}
void Player::UpdateFloatGimmick() {
	// 1フレームでのパラメータ加算値
	const float kFroatStep =
	    2.0f * static_cast<float>(3.14159265359) / static_cast<float>(kFloatAnimationCycle);
	// パラメータを1ステップ分加算
	floatingParameter_ += kFroatStep;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast<float>(3.14159265359));
	// 浮遊を座標に反映
	worldTransform_Motion_.translation_.y = (std::sin(floatingParameter_) * kFloatAmplitude);
}
void Player::Base() {
	// プレイヤーの回転イージング
	PlayerRotate();
	// 浮いてるモーション
	UpdateFloatGimmick();
}
void Player::ArmLeft() {
	// 浮遊を座標に反映
	worldTransforms_Parts_[static_cast<int>(Parts::ARML)].rotation_.x =
	    std::sin(floatingParameter_) * kArmRAmplitude;
}
void Player::ArmRight() { // 1フレームでのパラメータ加算値
	// 浮遊を座標に反映
	worldTransforms_Parts_[static_cast<int>(Parts::ARMR)].rotation_.x =
	    std::cos(floatingParameter_) * kArmLAmplitude;
}
void Player::Head() {}
void Player::Body() {}
void Player::BehaviorRootInitialize() {InitializeFloatGimmick();}