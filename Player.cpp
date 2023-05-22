#include "Player.h"
#include "MyMath.h"
#include "mat4x4.h"
#include <ImGuiManager.h>
#include <cassert>

#include "GameScene.h"

Player::~Player() { delete sprite2DReticle_; }

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(0.0f, 0.0f, 50.0f);
	worldTransform_.UpdateMatrix();
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {0.0f, 0.0f,}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection& viewProjection) {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 回転の速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
#pragma region 弾の寿命確認
	// デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});
#pragma endregion 弾の寿命確認
#pragma region キーボード入力
#pragma region キャラクター移動
	// 押した方向で良そうベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
#pragma endregion キャラクター移動
#pragma region キャラクター旋回
	// 押した方向で良そうベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	// 押した方向で稼働ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
#pragma endregion キャラクター旋回
#pragma region キャラクター攻撃

#pragma region 3Dレティクル
	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルのオフセット(Z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の回転を反映
	mat4x4 playerMatWorld = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	offset = TransformNormal(offset,Convert(playerMatWorld));
	offset.Normalize();
	offset = offset * kDistancePlayerTo3DReticle;
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = offset + worldTransform_.translation_;
	worldTransform3DReticle_.UpdateMatrix();
#pragma endregion 3Dレティクル
	Attack();

#pragma endregion キャラクター攻撃
#pragma endregion キーボード入力
#pragma region 移動限界座標
	// 移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	// 範囲を超えないように処理
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

#pragma endregion 移動限界座標
#pragma region 座標変換
	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();
#pragma endregion 座標変換
#pragma region 攻撃アップデート
	for (std::list<std::unique_ptr<PlayerBullet>>::iterator it = bullets_.begin();
	     it != bullets_.end(); ++it) {
		(*it)->Update();
	}
#pragma endregion 攻撃アップデート
#pragma region 3Dレティクル
	POINT mousePosition;
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	/*worldTransform3DReticle_.translation_ = Vector3(
	    static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y),
	    worldTransform3DReticle_.translation_.z);*/
	{ 
		mat4x4 reticleWorldPos = MakeAffineMatrix(
		    worldTransform3DReticle_.scale_, 
			worldTransform3DReticle_.rotation_,
		    worldTransform3DReticle_.translation_);
		Vector3 positionReticle = Vector3(reticleWorldPos.m[3][0], reticleWorldPos.m[3][1], reticleWorldPos.m[3][2]);

		// ビューポート行列
		mat4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		mat4x4 matViewProjectionViewport =
		    Convert(viewProjection.matView) * Convert(viewProjection.matProjection) * matViewport;

		// ワールド->スクリーン座標変換(ここで3Dから2Dになる)
		positionReticle = Transform(
		    Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), worldTransform3DReticle_.translation_.z),
		    matViewProjectionViewport);

		// スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


	}
#pragma endregion 3Dレティクル
#pragma region デバック
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::Text(
	    "position %f,%f,%f", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
#pragma endregion デバック
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::DrawUI() {
	// 3Dレティクルを描画
	sprite2DReticle_->Draw();
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		// 自キャラからレティクルのベクトルを計算
		Matrix4x4 playerMat = worldTransform_.matWorld_ * worldTransform_.parent_->matWorld_;
		Vector3 toReticle = worldTransform3DReticle_.translation_ -
		                    Vector3(playerMat.m[3][0], playerMat.m[3][1], playerMat.m[3][2]);

		toReticle.Normalize();
		toReticle *= kBulletSpeed;

		// 自キャラの座標をコピー
		WorldTransform tmp = worldTransform_;
		tmp.matWorld_ *= worldTransform_.parent_->matWorld_;
		tmp.UpdateMatrix();
		Vector3 position = {
			tmp.matWorld_.m[3][0], 
			tmp.matWorld_.m[3][1], 
			tmp.matWorld_.m[3][2]};

		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, position, toReticle);

		// 弾を登録する
		gameScene_->AddPlayerBullet(std::move(newBullet));
	}
}

Vector3 Player::GetWorldPosition() {
	return {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
}

void Player::OnCollision() {}