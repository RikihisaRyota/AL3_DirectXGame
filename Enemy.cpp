#include "Enemy.h"
#include <assert.h>
#include "mat4x4.h"
#include <imgui.h>

//staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::*Enemy::spUpdateTable[])() = {
    &Enemy::UpdateApproach,
    &Enemy::UpdateLeave,
};

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Enemy::Update() { 
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 

	//キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};

	

	//現在フェーズの関数を実行
	(this->*spUpdateTable[static_cast<size_t>(phase_)])();

	//座標移動
	worldTransform_.translation_ += move;

	UpdateMatrix();

	//デバック
	//  キャラクターの座標を画面表示する処理
	//  ウィンドウの位置を指定する変数
	ImVec2 window_pos = ImVec2(60.0f, 120.0f);

	// ウィンドウを描画する前に、ウィンドウの位置を指定する
	ImGui::SetNextWindowPos(window_pos);
	ImGui::Begin("Enemy");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	//  自作のAffine変換だと無理だから
	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection,textureHandle_);
}

void Enemy::UpdateApproach() {
	// キャラクターの移動速度
	const float kApproachSpeed = -0.2f;
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += Vector3(0.0f, 0.0f, kApproachSpeed);
	if (worldTransform_.translation_.z < -20.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::UpdateLeave() {
	// キャラクターの移動速度
	const float kLeaveSpeed = -0.2f;
	// 移動（ベクトルを加算）
	worldTransform_.translation_ += Vector3(kLeaveSpeed, -kLeaveSpeed, kLeaveSpeed);
}
