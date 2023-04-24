#include "Player.h"
#include <cassert>
#include "mat4x4.h"
#include <ImGuiManager.h>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
#pragma region キーボード入力
	//押した方向で良そうベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で良そうベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
#pragma endregion キーボード入力
#pragma region 移動限界座標
	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	//範囲を超えないように処理
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

#pragma endregion 移動限界座標
#pragma region 座標変換
	//座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	//自作のAffine変換だと無理だから
	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);
#pragma endregion 座標変換
#pragma region デバック
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
#pragma endregion デバック
}

void Player::Draw(ViewProjection& viewProjection) {
	
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}