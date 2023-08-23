#pragma once
#include <memory>
#include <vector>

#include "Collider.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータの配列</param>
	virtual void Initialize(std::vector<std::unique_ptr<Model>> model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

public:
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransform_Motion() const { return worldTransform_Motion_; }
	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns>ワールド変換データ</returns>
	const WorldTransform& GetWorldTransforms_Parts(int count) const { return worldTransforms_Parts_[count];}
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale;
	}
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation;
	}
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation;
	}
	void SetWorldtransform(const WorldTransform& worldtransfrom) {
		worldTransform_ = worldtransfrom;
		worldTransform_.UpdateMatrix();
	}
	void SetWorldtransform_Motion(const WorldTransform& worldtransfrom) {
		worldTransform_Motion_ = worldtransfrom;
		worldTransform_Motion_.UpdateMatrix();
	}
	void SetWorldtransforms_Parts(const WorldTransform& worldtransfrom, int count) {
		worldTransforms_Parts_[count] = worldtransfrom;
		worldTransforms_Parts_[count].UpdateMatrix();
	}

protected:
	// モデルデータ配列
	std::vector<std::unique_ptr<Model>> models_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ワールドトランスフォーム(アニメーション用)
	WorldTransform worldTransform_Motion_;
	// ワールドトランスフォームパーツごと
	std::vector<WorldTransform> worldTransforms_Parts_;
	// ベクトル
	Vector3 vector_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// AABB
	Vector3 min_;
	Vector3 max_;
	// OBB
	Vector3 size_;
	Vector3 center_;
	// Sphere
	float radius_;
};