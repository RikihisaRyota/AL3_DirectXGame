#include "Player.h"

#include <cassert>

#include "input/Input.h"
#include "MyMath.h"

void Player::Initialize(std::unique_ptr<Model> model) {
	// nullptrチェック
	assert(model);
	model_ = std::move(model);
	// worldTransform初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	// プレイヤーの動作
	Move();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::Move() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};

	// ゲームパットの状況取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動量
		Vector3 move = {
		    static_cast<float>(joyState.Gamepad.sThumbLX / SHRT_MAX * kSpeed),
		    0.0f,
		    static_cast<float>(joyState.Gamepad.sThumbLY / SHRT_MAX * kSpeed),
		};
		// 移動量に速さを反映
		if (move != Vector3(0.0f, 0.0f, 0.0f)) {
			move.Normalize();
		}
		move *= kSpeed;

		// 移動
		worldTransform_.translation_ += move;
	}

	// 行列を更新
	worldTransform_.UpdateMatrix();
}