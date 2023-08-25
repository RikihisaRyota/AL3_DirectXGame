#include "BaseCharacter.h"

void BaseCharacter::Initialize(std::vector<std::unique_ptr<Model>> model) {
	// モデル代入
	models_ = std::move(model);
	WorldTransform worldtransform;
	WorldTransform worldTransform_Motion;
	std::vector<WorldTransform> worldTransforms_Parts;

	// worldTransform_初期化
	worldtransform.Initialize();
	// アニメーション用のworldTransform_初期化
	worldTransform_Motion.Initialize();
	// パーツのworldTransforms_を初期化
	for (size_t partscount = 0; partscount < models_.size(); partscount++) {
		WorldTransform parts;
		parts.Initialize();
		worldTransforms_Parts.emplace_back(parts);
	}
	worldTransform_.emplace_back(worldtransform);
	worldTransform_Motion_.emplace_back(worldTransform_Motion);
	worldTransforms_Parts_.emplace_back(worldTransforms_Parts);

	worldTransform_Motion_.back().parent_ = &worldTransform_.back();
	for (size_t partscount = 0; partscount < worldTransforms_Parts_.at(0).size(); partscount++) {
		worldTransforms_Parts_.back().at(partscount).parent_ = &worldTransform_Motion_.back();
	}
	// ベクトル
	vector_ = {0.0f, 0.0f, 0.0f};
	// 速度
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 加速度
	acceleration_ = {0.0f, 0.0f, 0.0f};
}

void BaseCharacter::Update() {
	for (size_t i = 0; i < worldTransform_.size(); i++) {
		worldTransform_.at(i).UpdateMatrix();
		worldTransform_Motion_.at(i).UpdateMatrix();
		for (size_t model = 0; model < worldTransforms_Parts_.at(i).size(); model++) {
			worldTransforms_Parts_.at(i).at(model).UpdateMatrix();
		}
	}
}

//void BaseCharacter::Resize(size_t count) {
//	worldTransform_.resize(count);
//	worldTransform_Motion_.resize(count);
//	for (size_t i = 0; i < worldTransform_.size(); i++) {
//		// worldTransform_初期化
//		worldTransform_.at(i).Initialize();
//		// アニメーション用のworldTransform_初期化
//		worldTransform_Motion_.at(i).Initialize();
//		// 親子付け
//		worldTransform_Motion_.at(i).parent_ = &worldTransform_.at(i);
//		// サイズを決める
//		worldTransforms_Parts_.resize(models_.size());
//		// パーツのworldTransforms_を初期化
//		for (size_t model = 0; model < models_.size(); model++) {
//			worldTransforms_Parts_.at(i).at(model).Initialize();
//			worldTransforms_Parts_.at(i).at(model).parent_ = &worldTransform_Motion_.at(i);
//		}
//	}
//}
