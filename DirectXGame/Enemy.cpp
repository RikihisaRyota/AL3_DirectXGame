#include "Enemy.h"
#include "MyMath.h"
#include "mat4x4.h"
#include <assert.h>
#include <imgui.h>

#include "Player.h"
#include "GameScene.h"

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle,const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	state_ = new Approach;
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
	BulletsSet();
}

void Enemy::Update() {
	// キャラクターの移動ベクトル
	// 行動パターン
	UpdateState();

	// 座標移動
	worldTransform_.UpdateMatrix();

	// デバック
	//   キャラクターの座標を画面表示する処理
	//   ウィンドウの位置を指定する変数
	ImVec2 window_pos = ImVec2(60.0f, 120.0f);

	// ウィンドウを描画する前に、ウィンドウの位置を指定する
	ImGui::SetNextWindowPos(window_pos);
	ImGui::Begin("Enemy");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::OnCollision() {}

void Enemy::UpdateState() {
	state_->Update(this); 
}

void Enemy::Fire() {

	// 自キャラの座標をコピー
	Vector3 playerPosition = player_->GetWorldPosition();
	// ホーミングの初期ベクトル
	Vector3 vector = playerPosition - worldTransform_.translation_;
	// 正規化
	vector.Normalize();

	// 弾の速度
	const float kBulletSpeed = 0.5f;
	Vector3 velocity(vector * kBulletSpeed);

	// 速度ベクトルに自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->SetPlayer(player_);
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::BulletsSet() {
	Fire();
		
	// 発射タイマーをリセットする
	std::function<void(void)> callback = std::bind(&Enemy::BulletsSet, this);
	std::unique_ptr<TimeCall> timedCall = std::make_unique<TimeCall>(callback, kFireInterval);
	//gameScene_->AddEnemyBulletTimedCalls(std::move(timedCall));
}

Vector3 Enemy::GetWorldPosition() {
	return {
	    worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
}

void Enemy::changeState(EnemyState* newState) {
	state_ = newState;
}
