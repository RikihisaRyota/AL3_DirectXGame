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
	// worldTransforms初期化
	worldTransforms_.resize(models_.size()); // モデル数に合わせてサイズを調整
	for (size_t i = 0; i < models_.size(); ++i) {
		auto& worldTransform = worldTransforms_[i];
		worldTransform.Initialize();
		worldTransform.parent_ = &worldTransform_;
	}
	// 浮遊アニメーションの初期化
	InitializeFloatGimmick();
}

void Player::Update() {
	// プレイヤーの動作
	Move();
	// プレイヤーモーションの更新
	Motion();
}

void Player::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < models_.size(); i++) {
		models_[i]->Draw(worldTransforms_[i], viewProjection);
	}
}

void Player::Move() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// ゲームパットの状況取得
	// 入力がなかったら何もしない
	if (Input::GetInstance()->GetJoystickState(0, joyState) &&
	    (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0)) {
		// 移動量
		Vector3 move = {
		    static_cast<float>(joyState.Gamepad.sThumbLX),
		    0.0f,
		    static_cast<float>(joyState.Gamepad.sThumbLY),
		};
		// 移動量に速さを反映
		if (move != Vector3(0.0f, 0.0f, 0.0f)) {
			move.Normalize();
		}
		move *= kSpeed;
		// 移動ベクトルにカメラの回転角度だけ回転
		// 回転行列生成
		Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_->rotation_.y);
		// オフセットをカメラの回転に合わせて回転させる
		move = TransformNormal(move, rotate);
		// 移動
		worldTransform_.translation_ += move;
	}
}

void Player::PlayerRotate() {
	float rotate = std::atan2(move.x, move.z);
	//  Y軸回り角度(θy)
	worldTransform_.rotation_.y =
	    std::atan2(move.x, move.z); 
}

void Player::InitializeFloatGimmick() { 
	floatingParameter_ = 0.0f; 
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
	UpdateMotionMatrix();
}

void Player::UpdateMotionMatrix() {
	worldTransform_.UpdateMatrix();
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
	worldTransform_.translation_.y =
	    (std::sin(floatingParameter_) * kFloatAmplitude) + kGroundDistanse;
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