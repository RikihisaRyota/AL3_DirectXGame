#include "RailCamera.h"
#include "Vector3.h"
#include <imgui.h>

void RailCamera::Initialize(const Vector3* position, const Vector3* radian) {
	// ワールドトランスフォームの初期化
	worldTransform_.translation_ = *position;
	worldTransform_.rotation_ = *radian;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update(ViewProjection* viewProjection /*Vector3* position, Vector3* radian*/) {
	worldTransform_.translation_ += Vector3(0.0f,0.0f,-0.08f)/* *position*/;
	worldTransform_.rotation_ += Vector3(0.0f, 0.0f, 0.0f) /**radian*/;

	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);

	// カメラオブジェクトのワールド行列からビュー行列を計算する(きも)
	viewProjection->matView = Convert(Inverse(Convert(worldTransform_.matWorld_)));
	// ビュープロジェクションを転送
	viewProjection->TransferMatrix();

	// デバック
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("rotation_", &worldTransform_.rotation_.x, 0.0f, 1.0f);
	ImGui::End();
}

void RailCamera::Draw() {}
