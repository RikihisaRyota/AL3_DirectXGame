#include "Enemy.h"

void Enemy::Initialize(std::vector<std::unique_ptr<Model>> model) {
	BaseCharacter::Initialize(std::move(model));
}

void Enemy::Update() {}

void Enemy::Draw(const ViewProjection& viewProjection) { 
	BaseCharacter::Draw(viewProjection); 
}