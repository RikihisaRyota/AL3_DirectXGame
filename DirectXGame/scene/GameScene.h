#pragma once
#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "FollowCamera.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
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
	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
#pragma endregion
	/// <summary>
	/// ゲームシーン用
	/// </summary>
#pragma region プレイヤー
	// プレイヤー
	std::unique_ptr<Player> player_;
	// プレイヤーモデル
	std::unique_ptr<Model> playerModel_;
#pragma endregion
#pragma region 天球
	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 天球モデル
	std::unique_ptr<Model> skydomeModel_;
#pragma endregion
#pragma region 地面
	// 地面
	std::unique_ptr<Skydome> ground_;
	// 地面モデル
	std::unique_ptr<Model> groundModel_;
#pragma endregion
#pragma region 追従カメラ
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
#pragma endregion
};
