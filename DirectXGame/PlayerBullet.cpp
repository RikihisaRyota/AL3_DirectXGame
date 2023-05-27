#include "PlayerBullet.h"
#include <assert.h>
#include <ImGuiManager.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.jpeg");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期設定をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数にセット
	velocity_ = velocity;
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Update() { 
	//座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	ImGui::Begin("PlayerBullets");
	ImGui::Text(
	    "worldTransform_Position %f,%f,%f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() { isDead_ = true; }
