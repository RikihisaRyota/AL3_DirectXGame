#pragma once
#include <cstdlib>
#include <memory>
#include <optional>

#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/Input.h"
class Player : public BaseCharacter {
public:
	// 体のパーツ
	enum class Parts { 
		HEAD, 
		BODY, 
		ARML, 
		ARMR,
		WEAPON,
		COUNT
	};
	// ふるまい
	enum class Behavior {
		kRoot, // 通常
		kAttack, // 攻撃中
		kDash, // ダッシュ中
	};

	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

private:
	WorkDash workDash_;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>> model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
private: // メンバ関数
#pragma region 移動系
	     /// <summary>
	/// ゲームパット入力
	/// </summary>
	void GamePadInput();
	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void Move();
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	/// <summary>
	/// 重力
	/// </summary>
	void Gravity();
#pragma endregion
#pragma region Motion系
	/// <summary>
	/// 動き
	/// </summary>
	void Motion();
	/// <summary>
	/// モーションで計算したものを転送
	/// </summary>
	void UpdateMotionMatrix();
	/// <summary>
	/// プレイヤーの体の回転
	/// </summary>
	void PlayerRotate();
	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatGimmick();
	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatGimmick();
	/// <summary>
	/// 全体
	/// </summary>
	void Base();
	/// <summary>
	/// 左腕
	/// </summary>
	void ArmLeft();
	/// <summary>
	/// 右腕
	/// </summary>
	void ArmRight();
	/// <summary>
	/// 頭
	/// </summary>
	void Head();
	/// <summary>
	/// 体
	/// </summary>
	void Body();
#pragma endregion
#pragma region RootBehavior
	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();
	/// <summary>
	///  通常行動更新
	/// </summary>
	void BehaviorRootUpdate();
	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();
	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
	/// <summary>
	/// ダッシュ行動初期化
	/// </summary>
	void BehaviorDashInitialize();
	/// <summary>
	/// ダッシュ行動更新
	/// </summary>
	void BehaviorDashUpdate();
#pragma endregion

	void SetGlobalVariables();
	void GetGlobalVariables();

public: // ゲッター,セッター
	/// <summary>
	/// ViewProjectionのセッター
	/// </summary>
	/// <param name="viewprojection"></param>
	void SetViewProjection(const ViewProjection* viewprojection) {
		viewProjection_ = viewprojection;
	}
private: // 定数系
	// 地面から距離
	const float kGroundDistanse = 1.0f;
	// キャラクターの速さ
	const float kSpeed = 0.15f;
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	// 浮遊アニメーションの周期
	const uint16_t kFloatAnimationCycle = 60;
	// 浮遊アニメーションの振幅
	const float kFloatAmplitude = 0.05f;
	// 左腕のアニメーションの振幅
	const float kArmLAmplitude = 0.5f;
	// 右腕のアニメーションの振幅
	const float kArmRAmplitude = 0.5f;
	// 振り向き速度
	const float kTurn = 0.4f;
	// 重力
	const float kGravity = 0.01f;
	// ジャンプ
	const float kJumpPower = 0.2f;
	// 摩擦
private: // メンバ変数
	// 最終的に向きたい方向
	Vector3 destinationAngle_;
	// ダッシュ用方向
	Vector3 dashAngle_;
	// 向き
	Vector3 interRotate_;
	// ジャンプフラグ
	bool isJump;
	// floatアニメーションのカウント
	float floatingParameter_;
	// ふるまい
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 剣のデットゾーン
	float slashMin_;
	float slashMax_;
	float slash_Attack_Min_;
	float slash_Attack_Max_;
	// 初めの角度
	float slash_Attack_Start_;
	float slash_ArmAngle_Start_;
	// 攻撃の溜めモーションスピード
	float charge_Speed_;
	float charge_T_; 
	// 溜めているかどうかのフラグ
	bool chargeFlag_ = false;
	// 降り下ろしモーション
	float slash_Speed_;
	float slash_T_;
	// 溜めてあと立てるフラグ
	bool slashFlag_ = false;
	// 攻撃硬直
	bool rigorFlag_ = false;
	float rigor_Speed_;
	float rigor_T_;
};