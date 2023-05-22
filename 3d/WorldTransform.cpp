#include "WorldTransform.h"
#include "mat4x4.h"

void WorldTransform::UpdateMatrix() {
		// スケール、回転、平行移動を合成して行列を計算する
		//  自作のAffine変換だと無理だから
		mat4x4 tmp;
	    tmp = MakeAffineMatrix(
	        scale_, 
			rotation_, 
			translation_);
		matWorld_ = Convert(tmp);
		//親があれば親のワールド行列を書ける
	    if (parent_) {
		    matWorld_ *= parent_->matWorld_;
		}
		// 定数バッファに転送する
		TransferMatrix();
}