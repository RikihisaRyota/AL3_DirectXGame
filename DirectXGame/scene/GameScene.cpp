#include "GameScene.h"
#include "AxisIndicator.h"
#include "MyMath.h"
#include "TextureManager.h"
#include <fstream>
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.png");

	//レティクルのテクスチャ
	TextureManager::Load("Reticle.png");
#pragma region 生成
	// 3Dモデルの生成
	model_ = Model::Create();

	// 自キャラも生成
	player_ = new Player();

	// 敵キャラ生成
	/*enemy_ = new Enemy();*/

	//コリジョンマネージャー生成
	collisionManager = std::make_unique<CollisionManager>();

	//skydome生成
	skydome = std::make_unique<Skydome>();


	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("sky", true);

	// レールカメラ生成
	railCamera_ = std::make_unique<RailCamera>();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
#pragma endregion

#pragma region 初期化
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの初期化
	player_->SetGameScene(this);
	player_->Initialize(model_, textureHandle_, viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	//enemy_->SetPlayer(player_);

	//// 敵キャラの初期化
	//enemy_->SetGameScene(this);
	//enemy_->Initialize(model_, textureHandle_);

	// skydome初期化
	skydome->Initialize(modelSkydome_);

	//レールカメラ初期化
	railCamera_->Initialize(&viewProjection_.translation_, &viewProjection_.rotation_);

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// catmullRomの初期化
	catmull_Rom = std::make_unique<Catmull_Rom>();
	catmull_Rom->Initialize();
#pragma endregion 
	// csvファイルの読み込み
	LoadEnemyPopData();

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	// csvファイルアップデート
	UpdateEnemyPopCommands();
	
	// プレイヤーアップデート
	player_->Update(viewProjection_);

	// プレイヤー弾アップデート
	for (std::list<std::unique_ptr<PlayerBullet>>::iterator it = playerBullets_.begin();
	     it != playerBullets_.end(); ++it) {
		(*it)->Update();
	}

	// 敵キャラアップデート
	for (std::list<std::unique_ptr<Enemy>>::iterator it = enemy_.begin(); it != enemy_.end();
	     ++it) {
		(*it)->Update();
	}

	// 敵弾アップデート
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator it = enemyBullets_.begin();
	     it != enemyBullets_.end(); ++it) {
		(*it)->SetPlayer(player_);
		(*it)->Update();
	}
	// 弾のタイマーの更新
	for (std::list<std::unique_ptr<TimeCall>>::iterator it = timedCalls_.begin();
	     it != timedCalls_.end(); ++it) {
		(*it)->Update();
	}
	// 寿命
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
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

	// 当たり判定
	collisionManager->Update(player_, playerBullets_,enemy_, enemyBullets_);

	skydome->Update();

	// レールカメラアップデート
	railCamera_->Update(&viewProjection_ /*&translation, &rotation*/);

	// デバックカメラアップデート
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (!isDebugCameraActive_) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		//// レールカメラアップデート
		//railCamera_->Update(&viewProjection_ /*&translation, &rotation*/);
	}
#endif // _DEBUG


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
	//skydomeの描画
	skydome->Draw(viewProjection_);

	// 敵弾描画
	for (std::list<std::unique_ptr<EnemyBullet>>::iterator it = enemyBullets_.begin();
	     it != enemyBullets_.end(); ++it) {
		(*it)->Draw(viewProjection_);
	}
	// 自弾描画
	for (std::list<std::unique_ptr<PlayerBullet>>::iterator it = playerBullets_.begin();
	     it != playerBullets_.end(); ++it) {
		(*it)->Draw(viewProjection_);
	}
	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵キャラの描画
	for (std::list<std::unique_ptr<Enemy>>::iterator it = enemy_.begin(); it != enemy_.end();
	     ++it) {
		(*it)->Draw(viewProjection_);
	}
	catmull_Rom->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/// // PlayerUI
	player_->DrawUI();

	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	// リストに追加する
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::AddEnemyBulletTimedCalls(std::unique_ptr<TimeCall> timeCall) {
	// リストに追加する
	timedCalls_.push_back(std::move(timeCall));
}

void GameScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet) {
	// リストに追加する
	playerBullets_.push_back(std::move((playerBullet)));
}

void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources./enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() { 
	// 待機処理
	if (waitFlag_) {
		waitTime_--;
		if (waitTime_ <= 0) {
		// 待機完了
			waitFlag_ = false;
		}
		return;
	}
	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
		// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			enemy->SetPlayer(player_);
			enemy->SetGameScene(this);
			enemy->Initialize(model_, textureHandle_, Vector3(x, y, z));
			enemy_.push_back(std::move(enemy));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}