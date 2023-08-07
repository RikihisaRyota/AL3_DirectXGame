#pragma once
#include "BaseCharacter.h"
#include "Collider.h"
class Player;
class PlayerAttack : public BaseCharacter, public Collider {
public:
	enum class Parts {
		WEAPON,
		COUNT,
	};

public:
	/// <summary>
	/// 初期化（モデル代入）
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>> model) override;
	/// <summary>
	/// 値の初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void HitBoxInitialize() override;
	void HitBoxDraw(const ViewProjection& viewProjection) override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision(const OBB& obb) override;

	void SetPlayer(Player* player) { player_ = player; }
private:
	void HitBoxUpdate() override;
	Player* player_;

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
