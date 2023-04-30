#include "Enemy.h"
#include "MyMath.h"
#include "mat4x4.h"
#include <assert.h>
#include <imgui.h>

////staticで宣言したメンバ関数ポインタテーブルの実体
// void (Enemy::*Enemy::spUpdateTable[])() = {
//     &Enemy::UpdateApproach,
//     &Enemy::UpdateLeave,
// };

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(20.0f, 10.0f, 50.0f);
	state_ = new Approach;
	BulletsSet();
}

void Enemy::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// キャラクターの移動ベクトル

	// 寿命
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	timedCalls_.remove_if([](std::unique_ptr<TimeCall>& timedCalls) {
		if (timedCalls->IsDead()) {
			return true;
		}
		return false;
	});

	// 行動パターン
	UpdateState();

	// 弾の更新
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator it = bullets_.begin();
	     it != bullets_.end(); ++it) {
		(*it)->Update();
	}
	// 弾のタイマーの更新
	for (std::list<std::unique_ptr<TimeCall>>::iterator it = timedCalls_.begin();
	     it != timedCalls_.end(); ++it) {
		(*it)->Update();
	}

	// 座標移動
	UpdateMatrix();

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

void Enemy::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	//  自作のAffine変換だと無理だから
	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator it = bullets_.begin();
	     it != bullets_.end(); ++it) {
		(*it)->Draw(viewProjection);
	}
}

void Enemy::UpdateState() {
	state_->Update(this); 
}

void Enemy::Fire() {
	// 自キャラの座標をコピー
	Vector3 position = worldTransform_.translation_;

	// 弾の速度
	const float kBulletSpeed = -2.0f;
	Vector3 velocity(0.0f, 0.0f, kBulletSpeed);

	// 速度ベクトルに自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::BulletsSet() {
	Fire();
		
	// 発射タイマーをリセットする
	std::function<void(void)> callback = std::bind(&Enemy::BulletsSet, this);
	std::unique_ptr<TimeCall> timedCall = std::make_unique<TimeCall>(callback, kFireInterval);
	timedCalls_.push_back(std::move(timedCall));
}

void Enemy::changeState(EnemyState* newState) {
	state_ = newState;
}
