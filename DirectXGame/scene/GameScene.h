#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"
#include "Skydome.h"

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
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	ViewProjection viewProjection_;
	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// プレイヤー
	std::unique_ptr<Player> player_;
	// プレイヤーモデル
	std::unique_ptr<Model> playerModel_;
	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 天球モデル
	std::unique_ptr<Model> skydomeModel_;
	// 地面
	std::unique_ptr<Skydome> ground_;
	// 地面モデル
	std::unique_ptr<Model> groundModel_;
};
