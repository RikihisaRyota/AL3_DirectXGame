#include "Player.h"

#include <cassert>

void Player::Initialize(Model* model, int32_t texture) { 
	// nullptrチェック
	assert(model);
	model_ = std::make_unique<Model>(model);
	// テクスチャハンドル
	texture_ = texture;
	// worldTransform初期化
	worldTransform_.Initialize(); 
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
