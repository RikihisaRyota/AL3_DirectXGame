#include "BaseCharacter.h"

void BaseCharacter::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// モデル代入
	models_ = std::move(model);
	// worldTransform_初期化
	worldTransform_.Initialize();
	// アニメーション用のworldTransform_初期化
	worldTransform_Motion_.Initialize();
	worldTransform_Motion_.parent_ = &worldTransform_;
	for (size_t i = 0; i < models_.size(); i++) {
		worldTransforms_Parts_[i].Initialize();
	}
}
void BaseCharacter::Update() { 
	worldTransform_.UpdateMatrix();
	for (size_t i = 0; i < worldTransforms_Parts_.size(); i++) {
		worldTransforms_Parts_[i].UpdateMatrix();
	}
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < worldTransforms_Parts_.size(); i++) {
		models_[i]->Draw(worldTransforms_Parts_[i], viewProjection);
	}
}