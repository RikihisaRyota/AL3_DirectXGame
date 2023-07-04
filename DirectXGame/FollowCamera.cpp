#include "FollowCamera.h"

#include "ImGuiManager.h"
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
			// X軸
			viewProjection_.rotation_.x -=
			    static_cast<float>(joyState.Gamepad.sThumbRY) * kRotateSpeedX;
			viewProjection_.rotation_.x = Clamp(
			    viewProjection_.rotation_.x, 
				DegToRad(deadZoneRotateMin),
			    DegToRad(deadZoneRotateMax));
			// Y軸
			viewProjection_.rotation_.y +=
			    static_cast<float>(joyState.Gamepad.sThumbRX) * kRotateSpeedY;
			// 回転行列生成
			Matrix4x4 rotate = Mul(
				MakeRotateXMatrix(viewProjection_.rotation_.x),
			    MakeRotateYMatrix(viewProjection_.rotation_.y));
			// オフセットをカメラの回転に合わせて回転させる
			offset = TransformNormal(offset, rotate);
		}
		// 座標をコピーしてずらす
		viewProjection_.translation_.x = target_->translation_.x + offset.x;
		viewProjection_.translation_.y = offset.y;
		viewProjection_.translation_.z = target_->translation_.z + offset.z;
	}
	ImGui::Begin("camera");
	ImGui::Text(
	    "x:%f,y:%f,z:%f",
		viewProjection_.translation_.x,
		viewProjection_.translation_.y,
	    viewProjection_.translation_.z);
	ImGui::End();
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}
