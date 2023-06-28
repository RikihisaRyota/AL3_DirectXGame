#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();

	// プレイヤーモデル
	playerModel_.reset(Model::Create());
	// プレイヤーテクスチャハンドル
	playerTextureHandle_ = TextureManager::Load("mario.png");
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	// プレイヤー初期化
	player_->Initialize(std::move(playerModel_),playerTextureHandle_);

	// 天球のモデル
	skydomeModel_.reset(Model::CreateFromOBJ("sky",true));
	// 天球生成
	skydome_ = std::make_unique<Skydome>();
	// 天球初期化
	skydome_->Initialize(std::move(skydomeModel_));

	//　地面のモデル
	groundModel_.reset(Model::CreateFromOBJ("sky", true));
	// 地面生成
	ground_ = std::make_unique<Skydome>();
	// 地面初期化
	ground_->Initialize(std::move(groundModel_));
}

void GameScene::Update() {}

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
