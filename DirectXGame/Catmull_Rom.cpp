#include "Catmull_Rom.h"
#include "MyMath.h"
#include <directxmath.h>

using namespace DirectX;

void Catmull_Rom::Initialize() {
	drawer_ = PrimitiveDrawer::GetInstance();
	controlPoints_ = {
	    {0.0f,  0.0f,  0.0f},	//!< 0
	    {10.0f, 10.0f, 10.0f},	//!< 1
	    {10.0f, 15.0f, 10.0f},	//!< 2
	    {20.0f, 15.0f, 10.0f},	//!< 3
	    {20.0f, 0.0f,  30.0f},	//!< 4
	    {30.0f, 0.0f,  30.0f},	//!< 5
	    {0.0f,  0.0f,  30.0f},	//!< 6
	    {10.0f, 10.0f, 20.0f},	//!< 7
	    {10.0f, 15.0f, 20.0f},	//!< 8
	    {20.0f, 15.0f, 20.0f},	//!< 9
	    {20.0f, 0.0f,  10.0f},	//!< 10
	    {30.0f, 0.0f,  0.0f},	//!< 11
	    {0.0f,  0.0f,  0.0f },	//!< 12
	};
}

void Catmull_Rom::Draw(const ViewProjection& viewProjection) {
	// 線分を描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing;
	// 線分の数
	const size_t segmentCount = 100;
	// 線分の数+1個分の頂点座標を計算

	for (std::vector<Vector3>::iterator itr = controlPoints_.begin();
	     itr != controlPoints_.end() - 4; ++itr) {
		for (size_t i = 0; i < segmentCount + 1; i++) {
			float t = 1.0f / segmentCount * i;
			Vector3 pos = CatmullRom(
			    Vector3(itr->x, itr->y, itr->z), 
				Vector3((itr + 1)->x, (itr + 1)->y, (itr + 1)->z),
			    Vector3((itr + 2)->x, (itr + 2)->y, (itr + 2)->z),
			    Vector3((itr + 3)->x, (itr + 3)->y, (itr + 3)->z), t);
			pointsDrawing.push_back(pos);
		}
	}
	drawer_->SetViewProjection(&viewProjection);
	for (std::vector<Vector3>::iterator itr = pointsDrawing.begin(); itr != pointsDrawing.end() - 1;++itr) {
		drawer_->DrawLine3d(
		    Vector3(itr->x, itr->y, itr->z), 
			Vector3((itr + 1)->x, (itr + 1)->y, (itr + 1)->z),
		    Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}
