#include "Player.h"

#include <cassert>

void Player::Initialize(Model* model, int32_t texture) { 
	// nullptr�`�F�b�N
	assert(model);
	model_ = std::make_unique<Model>(model);
	// �e�N�X�`���n���h��
	texture_ = texture;
	// worldTransform������
	worldTransform_.Initialize(); 
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
