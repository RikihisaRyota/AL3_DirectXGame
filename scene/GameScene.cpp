#include "GameScene.h"
#include "AxisIndicator.h"
#include "MyMath.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.png");

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラも生成
	player_ = new Player();

	// 敵キャラ生成
	enemy_ = new Enemy();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 敵キャラの初期化
	enemy_->Initialize(model_, textureHandle_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	// プレイヤーアップデート
	player_->Update();

	// 敵キャラアップデート
	enemy_->Update();
	// 当たり判定
	CheckAllCollisions();
	// デバックカメラアップデート
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 自キャラの描画
	player_->Draw(viewProjection_);
	// 敵キャラの描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
#pragma region 自キャラと敵弾の当たり判定
	// 自キャラと敵弾全ての当たり判定
	for (std::list<std::unique_ptr<EnemyBullet>>::const_iterator it = enemyBullets.begin();
	     it != enemyBullets.end(); ++it) {
		// イテレーターを使用して敵弾の座標を取得する
		CheakCollisionPair(player_, it->get());
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自弾と敵キャラ全ての当たり判定
	for (std::list<std::unique_ptr<PlayerBullet>>::const_iterator it = playerBullets.begin();
	     it != playerBullets.end(); ++it) {
		// イテレーターを使用して敵弾の座標を取得する
		CheakCollisionPair(it->get(), enemy_);
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (std::list<std::unique_ptr<PlayerBullet>>::const_iterator playerIt = playerBullets.begin();
	     playerIt != playerBullets.end(); ++playerIt) {
		for (std::list<std::unique_ptr<EnemyBullet>>::const_iterator enemyIt = enemyBullets.begin();
		     enemyIt != enemyBullets.end(); ++enemyIt) {
			CheakCollisionPair((*playerIt).get(), (*enemyIt).get());
		}
	}
#pragma endregion
}

void GameScene::CheakCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 判定対象AとBの座標
	Vector3 posA, posB;
	// Aの座標
	posA = colliderA->GetWorldPosition();
	// Bの座標
	posB = colliderB->GetWorldPosition();

	float distance, radiusA, radiusB;
	double radius;
	distance = Distance(posA, posB);
	radius = std::pow(colliderA->GetRadius() + colliderB->GetRadius(), 2);
	radiusA = colliderA->GetRadius() ;
	radiusB = colliderB->GetRadius();

	// 球と球の当たり判定
	if (distance <= std::pow(colliderA->GetRadius() + colliderB->GetRadius(), 2)) {
		// 自キャラの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}