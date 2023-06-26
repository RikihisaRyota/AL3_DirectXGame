#include "Skydome.h"
#include <assert.h>

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {50.0f, 50.0f, 50.0f};
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_,viewProjection);
}
