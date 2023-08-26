#include "GameScene.h"

#include <cassert>

#include "AxisIndicator.h"
#include "TextureManager.h"

#include "Draw.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
#pragma region 初期設定
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	soundHandle_ = audio_->LoadWave("system.wav");

	// デバックカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// デバックカメラフラグ切り替え
	debugCameraFlag_ = false;
	// viewProjectionの初期化
	viewProjection_.Initialize();
	// DrawLineに必要なviewProjectionをセット
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
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
	ground_ = std::make_unique<Ground>();
	// 地面初期化
	ground_->Initialize(std::move(groundModel));
#pragma endregion
	// 生成
	player_ = std::make_unique<Player>();
	playerAttack_ = std::make_unique<PlayerAttack>();
	playerHP_ = std::make_unique<PlayerHP>();
	enemy_ = std::make_unique<Enemy>();
	enemyAttack_ = std::make_unique<EnemyAttack>();
	enemyHP_ = std::make_unique<EnemyHP>();
#pragma region 敵
	// 敵モデル
	std::vector<std::unique_ptr<Model>> enemyModel(static_cast<int>(Enemy::Parts::COUNT));
	// 敵攻撃モデル
	std::vector<std::unique_ptr<Model>> enemyAttackModel(
	    static_cast<int>(EnemyAttack::Parts::COUNT));
	// 敵モデル
	enemyModel[static_cast<int>(Enemy::Parts::BODY)].reset(
	    Model::CreateFromOBJ("enemy_Body", true));
	enemyModel[static_cast<int>(Enemy::Parts::HEAD)].reset(
	    Model::CreateFromOBJ("enemy_Head", true));
	enemyModel[static_cast<int>(Enemy::Parts::ARML)].reset(
	    Model::CreateFromOBJ("enemy_armL", true));
	enemyModel[static_cast<int>(Enemy::Parts::ARMR)].reset(
	    Model::CreateFromOBJ("enemy_armR", true));

	// 敵攻撃モデル
	enemyAttackModel[static_cast<int>(EnemyAttack::Parts::CIRCLE)].reset(
	    Model::CreateFromOBJ("enemy_Attack_Circle", true));
	enemyAttackModel[static_cast<int>(EnemyAttack::Parts::PLANE)].reset(
	    Model::CreateFromOBJ("enemy_Attack_Plane", true));
	enemyAttackModel[static_cast<int>(EnemyAttack::Parts::METEO)].reset(
	    Model::CreateFromOBJ("enemy_Meteo", true));
	// 敵初期化
	enemy_->Initialize(std::move(enemyModel));
	enemy_->SetEnemyAttack(enemyAttack_.get());
	// 敵攻撃初期化
	enemyAttack_->SetPlayerEnemy(player_.get(), enemy_.get());
	enemyAttack_->Initialize(std::move(enemyAttackModel));
	// 敵HP初期化
	uint32_t textureHandle = TextureManager::Load("white1x1.png");
	enemyHP_->Initialize(textureHandle, textureHandle);
#pragma endregion
#pragma region プレイヤー
	// プレイヤーモデル
	std::vector<std::unique_ptr<Model>> playerModel(static_cast<int>(Player::Parts::COUNT));
	std::vector<std::unique_ptr<Model>> playerAttackModel(
	    static_cast<int>(PlayerAttack::Parts::COUNT));
	// プレイヤーモデル
	playerModel[static_cast<int>(Player::Parts::HEAD)].reset(Model::CreateFromOBJ("head", true));
	playerModel[static_cast<int>(Player::Parts::BODY)].reset(Model::CreateFromOBJ("body", true));
	playerModel[static_cast<int>(Player::Parts::ARML)].reset(Model::CreateFromOBJ("armL", true));
	playerModel[static_cast<int>(Player::Parts::ARMR)].reset(Model::CreateFromOBJ("armR", true));
	playerModel[static_cast<int>(Player::Parts::WEAPON)].reset(
	    Model::CreateFromOBJ("player_Weapon", true));
	playerAttackModel[static_cast<int>(PlayerAttack::Parts::WEAPON)].reset(
	    Model::CreateFromOBJ("player_Weapon", true));
	// プレイヤー初期化
	player_->SetPlayerAttack(playerAttack_.get());
	player_->SetGround(ground_.get());
	uint32_t chage_Handle = TextureManager::Load("chageAttack.png");
	uint32_t triple_Handle = TextureManager::Load("tripleAttack.png");
	uint32_t dash_Handle = TextureManager::Load("dash.png");
	uint32_t white_Handle = TextureManager::Load("white1x1.png");
	uint32_t y_buttan_Handle = TextureManager::Load("y_buttan.png");
	uint32_t b_buttan_Handle = TextureManager::Load("b_buttan.png");
	uint32_t x_buttan_Handle = TextureManager::Load("x_buttan.png");
	player_->SetSprite(
	    chage_Handle, triple_Handle, dash_Handle, 
		white_Handle, 
		y_buttan_Handle, b_buttan_Handle,x_buttan_Handle);
	playerAttack_->SetPlayer(player_.get());
	playerAttack_->SetEnemy(enemy_.get());
	player_->Initialize(std::move(playerModel));
	playerAttack_->Initialize(std::move(playerAttackModel));
	playerHP_->Initialize(textureHandle, textureHandle);
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
	//// 一回だけアップデート
	// 追従カメラの更新
	followCamera_->Update();
	viewProjection_.SetViewProjection(followCamera_->GetViewProjection());
	viewProjection_.TransferMatrix();
#pragma endregion
}

void GameScene::Update() {
	// プレイヤーの更新
	playerAttack_->Update();
	playerHP_->Update();
	player_->Update();
	// 敵の更新
	enemyAttack_->Update();
	enemyHP_->Update();
	enemy_->Update();

	collisionManager.Update(player_.get(), playerAttack_.get(), enemy_.get(),enemyAttack_.get());

	skydome_->Update();
	ground_->Update();
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
	if (player_->GetBehavior() == Player::Behavior::kAttack) {
		playerAttack_->Draw(viewProjection_);
	}

	// 敵描画
	enemy_->Draw(viewProjection_);
	if (enemy_->GetBehavior() == Enemy::Behavior::kAttack) {
		enemyAttack_->Draw(viewProjection_);
	}
	//////////////Debug//////////////////
	player_->HitBoxDraw(viewProjection_);
	if (player_->GetBehavior() == Player::Behavior::kAttack) {
		playerAttack_->HitBoxDraw(viewProjection_);
	}
	enemy_->HitBoxDraw(viewProjection_);
	if (enemy_->GetBehavior() == Enemy::Behavior::kAttack) {
		enemyAttack_->HitBoxDraw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList, Sprite::BlendMode::kNormal);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();
	//test->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	playerHP_->Draw();
	enemyHP_->Draw();
	// test->Draw();
	//  スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
