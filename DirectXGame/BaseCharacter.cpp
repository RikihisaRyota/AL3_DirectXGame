#include "BaseCharacter.h"

void BaseCharacter::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// モデル代入
	models_ = std::move(model);
	// worldTransform_初期化
	worldTransform_.Initialize();
	// アニメーション用のworldTransform_初期化
	worldTransform_Motion_.Initialize();
	// 親子付け
	worldTransform_Motion_.parent_ = &worldTransform_;
	// サイズを決める
	worldTransforms_Parts_.resize(models_.size());
	// パーツのworldTransforms_を初期化
	for (size_t i = 0; i < models_.size(); i++) {
		worldTransforms_Parts_[i].Initialize();
		worldTransforms_Parts_[i].parent_ = &worldTransform_Motion_;
	}
	// ベクトル
	vector_ = {0.0f, 0.0f, 0.0f};
	// 速度
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 加速度
	acceleration_ = {0.0f, 0.0f, 0.0f};
}

void BaseCharacter::Update() { 
	worldTransform_.UpdateMatrix();
	worldTransform_Motion_.UpdateMatrix();
	for (size_t i = 0; i < worldTransforms_Parts_.size(); i++) {
		worldTransforms_Parts_[i].UpdateMatrix();
	}
}