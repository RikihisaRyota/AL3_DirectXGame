#include "Player.h"

#include <cassert>

void Player::Initialize(std::unique_ptr<Model> model, int32_t texture) { 
	// nullptrチェック
	assert(model);
	model_ = std::move(model);
	// テクスチャハンドル
	texture_ = texture;
	// worldTransform初期化
	worldTransform_.Initialize(); 
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texture_);
}