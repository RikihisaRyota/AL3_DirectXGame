#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cstdlib>
#include <memory>

class Player {
public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, int32_t texture = 0);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // �����o�ϐ�
	WorldTransform worldTransform_;
	std::unique_ptr<Model> model_;
	int32_t texture_;
};
