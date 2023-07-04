#include "WorldTransform.h"

#include "Matrix4x4.h"
#include "MyMath.h"

void WorldTransform::UpdateMatrix() { 
	// ワールド行列
	matWorld_ = MakeAffineMatrix(
		scale_,
		rotation_,
		translation_
	);
	if (parent_) {
		matWorld_ = Mul(matWorld_, parent_->matWorld_);
	}
	// 定数バッファに転送
	TransferMatrix();
}