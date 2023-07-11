#include "BaseCharacter.h"

void BaseCharacter::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// ���f�����
	models_ = std::move(model);
	// worldTransform_������
	worldTransform_.Initialize();
	// �A�j���[�V�����p��worldTransform_������
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