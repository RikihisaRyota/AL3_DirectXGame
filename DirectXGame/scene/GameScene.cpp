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
	// viewProjectionの初期化
	viewProjection_.Initialize();
#pragma endregion
#pragma region プレイヤー
	// プレイヤーモデル
	playerModel_.reset(Model::CreateFromOBJ("player", true));
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤー初期化
	player_->Initialize(std::move(playerModel_));
#pragma endregion
#pragma region 天球
	// 天球のモデル
	skydomeModel_.reset(Model::CreateFromOBJ("sky", true));
	// 天球生成
	skydome_ = std::make_unique<Skydome>();
	// 天球初期化
	skydome_->Initialize(std::move(skydomeModel_));
#pragma endregion
#pragma region 地面
	// 地面のモデル
	groundModel_.reset(Model::CreateFromOBJ("ground", true));
	// 地面生成
	ground_ = std::make_unique<Skydome>();
	// 地面初期化
	ground_->Initialize(std::move(groundModel_));
#pragma endregion
#pragma region 追従カメラ
	// 追従カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラ初期化
	followCamera_->Intialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransform());
	// プレイヤーにビュープロジェクションをセット
	player_->SetViewProjection(followCamera_->GetViewProjection());
#pragma endregion
}

void GameScene::Update() {
	// デバックカメラの更新
	debugCamera_->Update();
	viewProjection_.SetViewProjection(&debugCamera_->GetViewProjection());
	viewProjection_.TransferMatrix();
	// 追従カメラの更新
	followCamera_->Update();
	viewProjection_.SetViewProjection(followCamera_->GetViewProjection());
	viewProjection_.TransferMatrix();
	// プレイヤーの更新
	player_->Update();
	// 軸方向表示の表示を有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
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
