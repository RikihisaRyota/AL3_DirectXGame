#include "PlayerBullet.h"
#include <assert.h>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.jpeg");
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期設定をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { UpdateMatrix(); }

void PlayerBullet::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	//  自作のAffine変換だと無理だから
	mat4x4 tmp;
	tmp = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = Convert(tmp);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
