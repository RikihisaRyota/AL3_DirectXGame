#include "EnemyBullet.h"
#include <assert.h>
#include <ImGui.h>

#include "Player.h"
#include "MyMath.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("red.png");
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//Z軸方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
#pragma region 方法1
	// Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	// Y軸周りに-θy回す回転行列を計算
	mat4x4 tmp = MakeRotateYMatrix(-std::atan2(velocity.x, velocity.z));
	Vector3 velocityZ = Transform(velocity,tmp);
	// X軸回り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
#pragma endregion 方法1
#pragma region 方法2
	//// Y軸回り角度(θy)
	// worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
	//// 横軸方向の長さを求める
	// float velocityXZ = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
	//// X軸回り角度(θx)
	// worldTransform_.rotation_.x = std::atan2(-velocity.y, velocityXZ);
#pragma endregion 方法2
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数のセット
	velocity_ = velocity;

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Update() { 
	// 敵弾から自キャラへのベクトルを計算
	Matrix4x4 tmp = player_->GetMatWorld() * player_->GetParentMatWorld();
	Vector3 toPlayer =
	    Vector3(tmp.m[3][0], tmp.m[3][1], tmp.m[3][1]) - worldTransform_.translation_;

	//ベクトルを正規化する
	toPlayer.Normalize();
	//球面線形補間により、今の速度と自キャラへのベクトルの内挿し、新たな速度とする
	velocity_ = Slerp(velocity_, toPlayer, 0.2f)*0.5f;
	//キャラの方向に弾の向きを変える処理
	//  Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸回り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//  ウィンドウの位置を指定する変数
	ImVec2 window_pos = ImVec2(60.0f, 180.0f);

	ImGui::SetNextWindowPos(window_pos);
	ImGui::Begin("Enemy");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }
