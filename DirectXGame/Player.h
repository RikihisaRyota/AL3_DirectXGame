#pragma once
#include <cstdlib>
#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "input/Input.h"
class Player {
public:
	enum class Parts { 
		HEAD, 
		BODY, 
		ARML, 
		ARMR
	};
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>> model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ関数
	/// <summary>
	/// プレイヤーの動き
	/// </summary>
	void Move();

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatGimmick();
	/// <summary>
	/// 動き
	/// </summary>
	void Motion();
	/// <summary>
	/// モーションで計算したものを転送
	/// </summary>
	void UpdateMotionMatrix();
	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatGimmick();
	/// <summary>
	/// 全体
	/// </summary>
	void All();
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

public: // ゲッター,セッター
	/// <summary>
	/// WorldTransformのゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }
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
	const float kSpeed = 0.3f;
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
private: // メンバ変数
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ワールドトランスフォームパーツごと
	std::vector<WorldTransform> worldTransforms_;
	// モデル
	std::vector<std::unique_ptr<Model>> models_;
	// 浮遊ギミックの媒介変数
	float floatingParameter_;
};