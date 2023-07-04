#include "Player.h"

#include <cassert>

#include "ImGuiManager.h"
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
	// 入力がなかったら何もしない
	if (Input::GetInstance()->GetJoystickState(0, joyState)&&
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
		// キャラの方向に弾の向きを変える処理
		//  Y軸回り角度(θy)
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
	// 行列を更新
	worldTransform_.UpdateMatrix();
}