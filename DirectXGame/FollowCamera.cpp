#include "FollowCamera.h"

#include "ImGuiManager.h"
#include "MyMath.h"
#include "input/Input.h"
void FollowCamera::Intialize() {
	// ビュープロジェクション初期化
	viewProjection_.Initialize();
	// オフセット初期化
	offsetInitialize_ = {0.0f, 2.0f, -10.0f};
	// オフセット
	offset_ = offsetInitialize_;
}

void FollowCamera::Update() {
	// 追従者がいれば
	if (target_) {
		// 追従対象者からカメラまでのオフセット
		offset_ = offsetInitialize_;
		// ゲームパットのカメラ処理
		GamePad();
		// キーボードのカメラ処理
		Keyboard();
		// 回転を更新
		RotateUpdate();
	}
	ImGui::Begin("camera");
	ImGui::Text(
	    "x:%f,y:%f,z:%f", viewProjection_.translation_.x, viewProjection_.translation_.y,
	    viewProjection_.translation_.z);
	ImGui::End();
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

void FollowCamera::GamePad() {
	// ゲームパットの状態を得る変数
	XINPUT_STATE joyState{};
	// ゲームパットの状況取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// カメラの角度から回転行列を計算する
		// X軸
		viewProjection_.rotation_.x -=
		    static_cast<float>(joyState.Gamepad.sThumbRY) * kRotateSpeedX;
		viewProjection_.rotation_.x = Clamp(
		    viewProjection_.rotation_.x, DegToRad(kDeadZoneRotateMin),
		    DegToRad(kDeadZoneRotateMax));
		// Y軸
		viewProjection_.rotation_.y +=
		    static_cast<float>(joyState.Gamepad.sThumbRX) * kRotateSpeedY;
	}
}

void FollowCamera::Keyboard() {
	if (Input::GetInstance()->PushKey(DIK_RIGHTARROW)) {
		// Y軸を軸に回転
		viewProjection_.rotation_.y += kRotateSpeedY * kKeyboardCameraSpeed_X;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFTARROW)) {
		// Y軸を軸に回転
		viewProjection_.rotation_.y -= kRotateSpeedY * kKeyboardCameraSpeed_X;
	}
	if (Input::GetInstance()->PushKey(DIK_UPARROW)) {
		// X軸を軸に回転
		viewProjection_.rotation_.x += kRotateSpeedX * kKeyboardCameraSpeed_Y;
		viewProjection_.rotation_.x = Clamp(
		    viewProjection_.rotation_.x, DegToRad(kDeadZoneRotateMin),
		    DegToRad(kDeadZoneRotateMax));
	}
	if (Input::GetInstance()->PushKey(DIK_DOWNARROW)) {
		// X軸を軸に回転
		viewProjection_.rotation_.x -= kRotateSpeedX * kKeyboardCameraSpeed_Y;
		viewProjection_.rotation_.x = Clamp(
		    viewProjection_.rotation_.x, DegToRad(kDeadZoneRotateMin),
		    DegToRad(kDeadZoneRotateMax));
	}
}

void FollowCamera::RotateUpdate() {
	// 回転行列生成
	Matrix4x4 rotate =
	    Mul(MakeRotateXMatrix(viewProjection_.rotation_.x),
	        MakeRotateYMatrix(viewProjection_.rotation_.y));
	// オフセットをカメラの回転に合わせて回転させる
	offset_ = TransformNormal(offset_, rotate);
	// 座標をコピーしてずらす
	viewProjection_.translation_.x = target_->translation_.x + offset_.x;
	viewProjection_.translation_.y = target_->translation_.y + offset_.y;
	viewProjection_.translation_.z = target_->translation_.z + offset_.z;
}
