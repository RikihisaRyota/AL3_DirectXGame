#pragma once
#include <optional>

#include "BaseCharacter.h"
#include "Collider.h"

class Player;
class Enemy;
class PlayerAttack : public BaseCharacter, public Collider {
public:
	enum class Parts {
		WEAPON,
		COUNT,
	};

	// ふるまい
	enum class Behavior {
		kRoot, // 通常
		kChargeAttack, // 攻撃中
		kTripleAttack,   // ３段攻撃中
	};

public:
	/// <summary>
	/// 初期化（モデル代入）
	/// </summary>
	void Initialize(std::vector<std::unique_ptr<Model>> model) override;
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
	void OnCollision(const OBB& obb, uint32_t type) override;

	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemy(Enemy* enmey) { enemy_ =enmey; }
	void SetBehavior(const Behavior& behavior) { behaviorRequest_ = behavior; }

private:
	void ChageAttackInitialize();
	void TripleAttackInitialize();
	void ChageAttackUpdate();
	void TripleAttackUpdate();
	void HitBoxUpdate() override;
	void Homing();
private:
	Player* player_;
	Enemy* enemy_;

	// ふるまい
	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	Vector3 tripleAttackMin_;
	Vector3 tripleAttackMax_;
	Vector3 tripleAttackSize_;
	Vector3 center_Distance_;

	bool hitFlag_ = false;

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
	bool chargeFlag_;
	// 降り下ろしモーション
	float slash_Speed_;
	float slash_T_;
	// 溜めてあと立てるフラグ
	bool slashFlag_;
	// 攻撃硬直
	bool rigorFlag_;
	float rigor_Speed_;
	float rigor_T_;


	// ３段攻撃
	// 一回目
	bool firstFlag;
	float first_T_;
	float first_Speed_;
	float armAngleStart_;
	float armAngleMax_;
	float armSlideStart_;
	float armSlideMax_;
	float bodyAngleStart_;
	float bodyAngleMax_;
	// ２回目
	bool secondFlag;
	float second_T_;
	float second_Speed_;
	// 3回目
	bool thirdFlag;
	float third_T_;
	float third_Speed_;
	

	const uint32_t kFlagMax = 120u;
	uint32_t kFlagCount = 0u;
};
