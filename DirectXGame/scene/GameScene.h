#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Catmull_Rom.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "DebugCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBulletTimedCalls(std::unique_ptr<TimeCall> timeCall);

	/// <summary>
	/// プレイヤー弾を追加する
	/// </summary>
	/// <param name="playerBullet"></param>
	void AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデルデータ
	Model* model_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;
	// 自弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemy_;
	// 敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	// 時限発動リスト
	std::list<std::unique_ptr<TimeCall>> timedCalls_;

	// コリジョンマネージャー
	std::unique_ptr<CollisionManager> collisionManager = nullptr;

	// skydome
	std::unique_ptr<Skydome> skydome;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	std::unique_ptr<Catmull_Rom> catmull_Rom = nullptr;	

	// レールカメラ
	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	//デバックカメラ有効	
	bool isDebugCameraActive_ = false;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	// 敵発生フラグ
	bool waitFlag_ = false;
	// 待機中タイマー
	int32_t waitTime_;
};
