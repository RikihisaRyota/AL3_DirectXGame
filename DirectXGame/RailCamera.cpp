#include "RailCamera.h"
#include "Vector3.h"
#include <imgui.h>
#include "MyMath.h"
#include "Player.h"
void RailCamera::Initialize(const Vector3* position, const Vector3* radian,Player* player) {
	player_ = player;
	// ワールドトランスフォームの初期化
	worldTransform_.translation_ = *position;
	worldTransform_.rotation_ = *radian;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	drawer_ = PrimitiveDrawer::GetInstance();
	controlPoints_ = {
	    {0.0f,  0.0f,  0.0f}, //!< 0
	    {10.0f, 10.0f, 10.0f}, //!< 1
	    {10.0f, 15.0f, 15.0f}, //!< 2
	    {20.0f, 15.0f, 5.0f}, //!< 3
	    {20.0f, 0.0f,  2.0f}, //!< 4
	    {30.0f, 0.0f,  0.0f}, //!< 5
	};

	for (std::vector<Vector3>::iterator itr = controlPoints_.begin();
	     itr != controlPoints_.end() - 4; ++itr) {
		for (size_t i = 0; i < segmentCount + 1; i++) {
			float t = 1.0f / segmentCount * i;
			Vector3 pos = CatmullRom(
			Vector3(itr->x, itr->y, itr->z),
			Vector3((itr + 1)->x, (itr + 1)->y, (itr + 1)->z),
			Vector3((itr + 2)->x, (itr + 2)->y, (itr + 2)->z),
			Vector3((itr + 3)->x, (itr + 3)->y, (itr + 3)->z), t);
			points.push_back(pos);
		}
	}

}

void RailCamera::Update(ViewProjection* viewProjection /*Vector3* position, Vector3* radian*/) {

	worldTransform_.translation_ = points[catmullCount - 4];

	mat4x4 eye = MakeTranslateMatrix(worldTransform_.translation_);
	mat4x4 target = MakeTranslateMatrix(points[catmullCount - 1]);

	Vector3 forward = Vector3(target.m[3][0], target.m[3][1], target.m[3][2]) -
	                  Vector3(eye.m[3][0], eye.m[3][1], eye.m[3][2]);
	
	forward.Normalize();

	// Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	// Y軸周りに-θy回す回転行列を計算
	mat4x4 tmp = MakeRotateYMatrix(-std::atan2(forward.x, forward.z));
	Vector3 velocityZ = Transform(forward, tmp);
	// X軸回り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

	mat4x4 tmpmatWorld;
	tmpmatWorld = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.matWorld_ = Convert(tmpmatWorld);

	// カメラオブジェクトのワールド行列からビュー行列を計算する(きも)
	viewProjection->matView = Convert(Inverse(Convert(worldTransform_.matWorld_)));
	// ビュープロジェクションを転送
	viewProjection->TransferMatrix();

	if (catmullCount < points.size()) {
		catmullCount++;
	}
	// デバック
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("rotation_", &worldTransform_.rotation_.x, -1.0f, 1.0f);
	ImGui::End();
}

void RailCamera::Draw() {}
