#include "Player.h"

#include <cassert>

#include "ImGuiManager.h"
#include "MyMath.h"

void Player::Initialize(std::vector<std::unique_ptr<Model>> model) {
	models_ = std::move(model);
	// プレイヤーの初期位置をずらす
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.UpdateMatrix();
	// アニメーション用のworldTransform_
	worldTransformMotion_.Initialize();
	worldTransformMotion_.parent_ = &worldTransform_;
	// worldTransforms初期化
	worldTransforms_.resize(models_.size()); // モデル数に合わせてサイズを調整
	for (size_t i = 0; i < models_.size(); ++i) {
		auto& worldTransform = worldTransforms_[i];
		worldTransform.Initialize();
		worldTransform.parent_ = &worldTransformMotion_;
	}
	// ベクトル
	vector_ = {0.0f, 0.0f, 0.0f};
	// 速度
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 加速度
	acceleration_ = {0.0f, 0.0f, 0.0f};
	// 方向
	direction_ = {0.0f, 0.0f, 1.0f};
	// ジャンプフラグ
	isJump = false;
	// 浮遊アニメーションの初期化
	InitializeFloatGimmick();
}

void Player::Update() {
	// 入力
	GamePadInput();
	// プレイヤーモーションの更新
	Motion();

	ImGui::Begin("Player");
	ImGui::Text(
	    "translation_ x:%f,y:%f,z:%f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text(
	    "rotate_ x:%f,y:%f,z:%f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z);
	ImGui::Text("direction_:%f,y:%f,z:%f", direction_.x, direction_.y, direction_.z);
	ImGui::Text("vector_x:%f,y:%f,z:%f", vector_.x, vector_.y, vector_.z);
	ImGui::Text("velocity_:%f,y:%f,z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("acceleration_:%f,y:%f,z:%f", acceleration_.x, acceleration_.y, acceleration_.z);
	// ImGui::SliderFloat("Jump", &kJumpPower, 0.00f, 0.90f);
	// ImGui::SliderFloat("Gravity", &kGravity, 0.00f, 0.50f);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	ChackHitBox(worldTransform_, viewProjection, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	for (size_t i = 0; i < models_.size(); i++) {
		models_[i]->Draw(worldTransforms_[i], viewProjection);
	}
}

void Player::GamePadInput() {
	// プレイヤー移動
	Move();
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
		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0) {
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
	}
	// 回転行列生成
	Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_->rotation_.y);
	// オフセットをカメラの回転に合わせて回転させる
	vector_ = TransformNormal(vector_, rotate);
#pragma endregion
}
void Player::Jump() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// ゲームパットの状況取得
	// 入力がなかったら何もしない
	if ((Input::GetInstance()->GetJoystickState(0, joyState) && (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) ||
	    Input::GetInstance()->TriggerKey(DIK_SPACE) &&
	    !isJump) {
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
	if (direction_ != Vector3(0.0f, 0.0f, 0.0f)) {
		direction_.Normalize();
	}
	Vector3 rotate = Lerp(direction_, vector_, kTurn);
	//  Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(rotate.x, rotate.z);
	// プレイヤーの向いている方向
	direction_ = rotate;
}
void Player::InitializeFloatGimmick() { floatingParameter_ = 0.0f; }

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
	UpdateMotionMatrix();
}

void Player::UpdateMotionMatrix() {
	worldTransform_.UpdateMatrix();
	worldTransformMotion_.UpdateMatrix();
	for (auto worldTrabsform : worldTransforms_) {
		worldTrabsform.UpdateMatrix();
	}
}

void Player::UpdateFloatGimmick() {
	// 1フレームでのパラメータ加算値
	const float kFroatStep =
	    2.0f * static_cast<float>(3.14159265359) / static_cast<float>(kFloatAnimationCycle);
	// パラメータを1ステップ分加算
	floatingParameter_ += kFroatStep;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast<float>(3.14159265359));
	// 浮遊を座標に反映
	 worldTransformMotion_.translation_.y =
	     (std::sin(floatingParameter_) * kFloatAmplitude);
}

void Player::Base() {
	// プレイヤーの回転イージング
	PlayerRotate();
	// 浮いてるモーション
	UpdateFloatGimmick();
}

void Player::ArmLeft() {
	// 浮遊を座標に反映
	worldTransforms_[static_cast<int>(Parts::ARML)].rotation_.x =
	    std::sin(floatingParameter_) * kArmRAmplitude;
}

void Player::ArmRight() { // 1フレームでのパラメータ加算値
	// 浮遊を座標に反映
	worldTransforms_[static_cast<int>(Parts::ARMR)].rotation_.x =
	    std::cos(floatingParameter_) * kArmLAmplitude;
}

void Player::Head() {}

void Player::Body() {}