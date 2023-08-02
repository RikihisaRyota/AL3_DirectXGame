#include "GameScene.h"

#include <cassert>

#include "AxisIndicator.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
#pragma region 初期設定
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバックカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// デバックカメラフラグ切り替え
	debugCameraFlag_ = false;
	// viewProjectionの初期化
	viewProjection_.Initialize();
#pragma endregion
#pragma region プレイヤー
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤーモデル
	std::vector<std::unique_ptr<Model>> playerModel(static_cast<int>(Player::Parts::COUNT));
	// プレイヤーモデル
	playerModel[static_cast<int>(Player::Parts::HEAD)].reset(Model::CreateFromOBJ("head", true));
	playerModel[static_cast<int>(Player::Parts::BODY)].reset(Model::CreateFromOBJ("body", true));
	playerModel[static_cast<int>(Player::Parts::ARML)].reset(Model::CreateFromOBJ("armL", true));
	playerModel[static_cast<int>(Player::Parts::ARMR)].reset(Model::CreateFromOBJ("armR", true));
	playerModel[static_cast<int>(Player::Parts::WEAPON)].reset(Model::CreateFromOBJ("player_Weapon", true));
	// プレイヤー初期化
	player_->Initialize(std::move(playerModel));
#pragma endregion
#pragma region 天球
	// 天球モデル
	std::unique_ptr<Model> skydomeModel;
	// 天球のモデル
	skydomeModel.reset(Model::CreateFromOBJ("sky", true));
	// 天球生成
	skydome_ = std::make_unique<Skydome>();
	// 天球初期化
	skydome_->Initialize(std::move(skydomeModel));
#pragma endregion
#pragma region 地面
	// 地面モデル
	std::unique_ptr<Model> groundModel;
	// 地面のモデル
	groundModel.reset(Model::CreateFromOBJ("ground", true));
	// 地面生成
	ground_ = std::make_unique<Skydome>();
	// 地面初期化
	ground_->Initialize(std::move(groundModel));
#pragma endregion
#pragma region 敵
	// 敵生成
	enemy_ = std::make_unique<Enemy>();
	// 敵モデル
	std::vector<std::unique_ptr<Model>>	enemyModel(static_cast<int>(Enemy::Parts::COUNT));
	// 敵モデル
	enemyModel[static_cast<int>(Enemy::Parts::BODY)].reset(Model::CreateFromOBJ("enemy_Body", true));
	enemyModel[static_cast<int>(Enemy::Parts::LIGHT)].reset(Model::CreateFromOBJ("enemy_Light", true));
	// 敵初期化
	enemy_->Initialize(std::move(enemyModel));
#pragma endregion
#pragma region 追従カメラ
	// 追従カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラ初期化
	followCamera_->Intialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// 敵キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetEnemy(&enemy_->GetWorldTransform());
	// プレイヤーにビュープロジェクションをセット
	player_->SetViewProjection(followCamera_->GetViewProjection());
#pragma endregion
}

void GameScene::Update() {
	// プレイヤーの更新
	player_->Update();
	// 敵の更新
	enemy_->Update();
#pragma region カメラ関連
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		debugCameraFlag_ ^= true;
	}
	if (debugCameraFlag_) {
		// デバックカメラの更新
		debugCamera_->Update();
		viewProjection_.SetViewProjection(&debugCamera_->GetViewProjection());
		viewProjection_.TransferMatrix();

	} else {
		// 追従カメラの更新
		followCamera_->Update();
		viewProjection_.SetViewProjection(followCamera_->GetViewProjection());
		viewProjection_.TransferMatrix();
	}
#pragma endregion
#pragma region 軸
	// 軸方向表示の表示を有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#pragma endregion
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
	// 天球描画
	skydome_->Draw(viewProjection_);
	// 地面描画
	ground_->Draw(viewProjection_);
	// プレイヤー描画
	player_->Draw(viewProjection_);
	// 敵描画
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
