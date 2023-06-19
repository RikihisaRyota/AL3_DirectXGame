#include "Player.h"
#include "MyMath.h"
#include "mat4x4.h"
#include <ImGuiManager.h>
#include <cassert>

#include "GameScene.h"

Player::~Player() { delete sprite2DReticle_; }

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection& viewProjection) {
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
	// ビューポート行列
	mat4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	mat4x4 matVPV =
	    Convert(viewProjection.matView) * Convert(viewProjection.matProjection) * matViewport;
	// スクリーン座標系からワールド座標系
	reticlePosition_ = Transform(worldTransform_.translation_, matVPV);
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle, 
		{reticlePosition_.x,reticlePosition_.y},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	
	controlPoints_ = {
	    {0.0f,  0.0f,  0.0f}, //!< 0
	    {10.0f, 10.0f, 0.0f}, //!< 1
	    {10.0f, 15.0f, 0.0f}, //!< 2
	    {20.0f, 15.0f, 0.0f}, //!< 3
	    {20.0f, 0.0f,  0.0f}, //!< 4
	    {30.0f, 0.0f,  0.0f}, //!< 5
	};

	for (std::vector<Vector3>::iterator itr = controlPoints_.begin();
	     itr != controlPoints_.end() - 4; ++itr) {
		for (size_t i = 0; i < segmentCount + 1; i++) {
			float t = 1.0f / segmentCount * i;
			Vector3 pos = CatmullRom(
			    Vector3(itr->x, itr->y, itr->z), Vector3((itr + 1)->x, (itr + 1)->y, (itr + 1)->z),
			    Vector3((itr + 2)->x, (itr + 2)->y, (itr + 2)->z),
			    Vector3((itr + 3)->x, (itr + 3)->y, (itr + 3)->z), t);
			points.push_back(pos);
		}
	}
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
#pragma region キャラクター移動
#pragma region キーボード入力
	// 押した方向で良そうベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
#pragma endregion キーボード入力
#pragma region ゲームパット入力
	// ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState = {};

	// ジョイスティック状態を取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

#pragma endregion ゲームパット入力
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
#pragma region キャラクター攻撃
#pragma region 3Dレティクル
#pragma region マウスレティクル
	//POINT mousePosition;
	//// マウス座標(スクリーン座標)を取得する
	//GetCursorPos(&mousePosition);

	//// クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);
	//reticlePosition_.x = static_cast<float>(mousePosition.x);
	//reticlePosition_.y =static_cast<float>(mousePosition.y);
	//// スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(reticlePosition_.x, reticlePosition_.x));
#pragma endregion マウスレティクル
#pragma region スティックレティクル
	// スプライトの現在座標を取得
	reticlePosition_.x = sprite2DReticle_->GetPosition().x;
	reticlePosition_.y = sprite2DReticle_->GetPosition().y;

	// ジョイスティック状態の取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		reticlePosition_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		reticlePosition_.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(Vector2(reticlePosition_.x, reticlePosition_.y));
	}
#pragma endregion スティックレティクル

	// ビューポート行列
	mat4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	mat4x4 matVPV =
	    Convert(viewProjection.matView) * Convert(viewProjection.matProjection) * matViewport;

	// 合成行列の逆行列を計算する
	mat4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(
	    static_cast<float>(reticlePosition_.x), static_cast<float>(reticlePosition_.y), 0.0f);

	Vector3 posFar = Vector3(
	    static_cast<float>(reticlePosition_.x), static_cast<float>(reticlePosition_.y), 1.0f);

	// スクリーン座標系からワールド座標系
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;

	mouseDirection.Normalize();

	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;

	Vector3 reticlePos = posNear + (mouseDirection * kDistanceTestObject);

	worldTransform3DReticle_.translation_ = Vector3(reticlePos.x, reticlePos.y, reticlePos.z);

	worldTransform3DReticle_.matWorld_ = Convert(MakeAffineMatrix(
	    worldTransform3DReticle_.scale_, 
		worldTransform3DReticle_.rotation_,
	    worldTransform3DReticle_.translation_));

	worldTransform3DReticle_.UpdateMatrix();

#pragma endregion 3Dレティクル
	Attack();
#pragma endregion キャラクター攻撃
#pragma region デバック
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "worldTransform_Position %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::Text(
	    "worldTransform_Position %f,%f,%f", worldTransform_.matWorld_.m[3][0],
	    worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]);
	ImGui::Text(
	    "sprite2DReticle_Position %f,%f", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near %f,%f,%f", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far %f,%f,%f", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "worldTransform3DReticle_Position %f,%f,%f", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::Text(
	    "worldTransform_Position %f,%f,%f", worldTransform3DReticle_.matWorld_.m[3][0],
	    worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2]);
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
	// ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState = {};
	Input::GetInstance()->GetJoystickState(0, joyState);

	if (input_->TriggerKey(DIK_SPACE) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// 弾の速度
		const float kBulletSpeed = 2.0f;
		// 自キャラからレティクルのベクトルを計算
		Matrix4x4 playerMat = worldTransform_.matWorld_;

		Vector3 toReticle = worldTransform3DReticle_.translation_ -
		                    Vector3(playerMat.m[3][0], playerMat.m[3][1], playerMat.m[3][2]);

		toReticle.Normalize();
		toReticle *= kBulletSpeed;

		// 自キャラの座標をコピー
		WorldTransform tmp = worldTransform_;
		tmp.matWorld_ *= worldTransform_.parent_->matWorld_;
		tmp.UpdateMatrix();
		Vector3 position = {tmp.matWorld_.m[3][0], tmp.matWorld_.m[3][1], tmp.matWorld_.m[3][2]};

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