#pragma once
#include "Audio.h"
#include "CollisionManager.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "EnemyAttack.h"
#include "EnemyHP.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "PlayerAttack.h"
#include "PlayerHP.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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
private: // メンバ変数
#pragma region 初期設定
	// 初期設定
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	ViewProjection viewProjection_;

	uint32_t soundHandle_;
	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// デバックカメラ切り替え
	bool debugCameraFlag_;
	CollisionManager collisionManager;

#pragma endregion
	/// <summary>
	/// ゲームシーン用
	/// </summary>
#pragma region プレイヤー
	// プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<PlayerAttack> playerAttack_;
	std::unique_ptr<PlayerHP> playerHP_;
#pragma endregion
#pragma region 天球
	// 天球
	std::unique_ptr<Skydome> skydome_;
#pragma endregion
#pragma region 地面
	// 地面
	std::unique_ptr<Ground> ground_;
#pragma endregion
#pragma region 追従カメラ
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
#pragma endregion
#pragma region 敵
	// 敵
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<EnemyAttack> enemyAttack_;
	std::unique_ptr<EnemyHP> enemyHP_;
#pragma endregion
};
