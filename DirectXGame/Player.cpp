#include "Player.h"

#include <cassert>

void Player::Initialize(std::unique_ptr<Model> model) { 
	// nullptrチェック
	assert(model);
	model_ = std::move(model);
	// worldTransform初期化
	worldTransform_.Initialize(); 
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.UpdateMatrix();
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}