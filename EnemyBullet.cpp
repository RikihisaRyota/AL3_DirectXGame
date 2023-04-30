#include "EnemyBullet.h"
#include <assert.h>
#include <ImGui.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数のセット
	velocity_ = velocity;
}

void EnemyBullet::Update() { 
	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;
	UpdateMatrix();
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//  ウィンドウの位置を指定する変数
	ImVec2 window_pos = ImVec2(60.0f, 180.0f);

	ImGui::SetNextWindowPos(window_pos);
	ImGui::Begin("Enemy");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void EnemyBullet::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	//  自作のAffine変換だと無理だから
	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
