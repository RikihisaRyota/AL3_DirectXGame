#include "Enemy.h"

#include "ImGuiManager.h"
#include "MyMath.h"

void Enemy::Initialize(std::vector<std::unique_ptr<Model>> model) {
	BaseCharacter::Initialize(std::move(model));
	// worldTransform_をずらす
	worldTransform_.translation_ = Vector3(0.0f, 1.0f, 10.0f);
	worldTransform_.UpdateMatrix();
	vector_ = Normalize(worldTransform_.translation_);
	// 変数初期化
	direction_ = worldTransform_.translation_;
	moveRatate_ = 0.0f;
	motionRatate_ = 0.0f;
}

void Enemy::Update() {
	// 移動
	Move();
	// 動き
	Motion();
	// 転送
	BaseCharacter::Update();
#ifdef DEBUG
	ImGui::Begin("Enemy");
	ImGui::Text(
	    "translation_ x:%f,y:%f,z:%f", worldTransform_.translation_.x,
	    worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::Text(
	    "rotate_ x:%f,y:%f,z:%f", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z);
	ImGui::Text("vector_x:%f,y:%f,z:%f", vector_.x, vector_.y, vector_.z);
	ImGui::Text("velocity_:%f,y:%f,z:%f", velocity_.x, velocity_.y, velocity_.z);
	ImGui::Text("acceleration_:%f,y:%f,z:%f", acceleration_.x, acceleration_.y, acceleration_.z);
	ImGui::Text(
	    "rotation_ x:%f,y:%f,z:%f",
	    worldTransforms_Parts_[static_cast<int>(Parts::LIGHT)].rotation_.x,
	    worldTransforms_Parts_[static_cast<int>(Parts::LIGHT)].rotation_.y,
	    worldTransforms_Parts_[static_cast<int>(Parts::LIGHT)].rotation_.z);
	ImGui::End();
#endif // DEBUG
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	for (size_t i = 0; i < worldTransforms_Parts_.size(); i++) {
		models_[i]->Draw(worldTransforms_Parts_[i], viewProjection);
	}
}

void Enemy::Move() {
	vector_ = {0.0f, 0.0f, 0.0f};
	// 円運動の計算
	float radius = 5.0f;
	float x = radius * std::cos(angle_);
	float z = radius * std::sin(angle_);
	vector_ = {x, 0.0f, z};
	vector_.Normalize();
	// 移動量に速さを反映
	if (vector_ != Vector3(0.0f, 0.0f, 0.0f)) {
		vector_.Normalize();
	}
	velocity_ = vector_ * 0.2f;
	velocity_ += acceleration_;
	worldTransform_.translation_ += velocity_;
	// 角度の更新
	angle_ += 0.02f;
}

void Enemy::Motion() {
	// 全体
	Base();
	// 体
	Body();
	// ライト
	Light();
}

void Enemy::Base() {
	//  Y軸回り角度(θy)
	worldTransform_Motion_.rotation_.y = std::atan2(vector_.x, vector_.z);
}

void Enemy::Body() {}

void Enemy::Light() {
	//  Y軸回り角度(θy)
	worldTransforms_Parts_[static_cast<int>(Parts::LIGHT)].rotation_.y += 0.08f;
}
