#include "FollowCamera.h"

#include "input/Input.h"
#include "MyMath.h"
void FollowCamera::Intialize() {
	// ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	// 追従者がいれば
	if (target_) {
		// 追従対象者からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		// ゲームパットの状態を得る変数
		XINPUT_STATE joyState{};
		// ゲームパットの状況取得
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// カメラの角度から回転行列を計算する
			Matrix4x4 rotate;
			// X軸
			viewProjection_.rotation_.x +=
			    static_cast<float>(joyState.Gamepad.sThumbRY) * kRotateSpeed;
			// Y軸
			viewProjection_.rotation_.y +=
			    static_cast<float>(joyState.Gamepad.sThumbRX) * kRotateSpeed;
			rotate = Mul(
				MakeRotateXMatrix(viewProjection_.rotation_.x),
			    MakeRotateYMatrix(viewProjection_.rotation_.y));
			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotate);
		}
		// 座標をコピーしてずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}
