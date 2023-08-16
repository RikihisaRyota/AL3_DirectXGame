#include "EnemyHP.h"

#include "ImGuiManager.h"
#include "MyMath.h"

void EnemyHP::Initialize(uint32_t now_TextureHandle, uint32_t delay_TextureHandle) {
	now_HP_ = kMax_HP_;
	delay_HP_ = kMax_HP_;
	add_HP_ = 0;
	now_HP_Sprite_ = std::make_unique<Sprite>();
	delay_HP_Sprite_ = std::make_unique<Sprite>();
	now_HP_Sprite_.reset(Sprite::Create(now_TextureHandle, Vector2(0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
	delay_HP_Sprite_.reset(Sprite::Create(delay_TextureHandle, Vector2(0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
}

void EnemyHP::Update() { 
	now_HP_ -= add_HP_;
	now_HP_Sprite_->SetSize(Vector2(static_cast<float>(Conversion(now_HP_)), kHeight_));
	delay_HP_ = static_cast<uint32_t>(Lerp(Conversion(now_HP_), Conversion(delay_HP_), 0.02f));
	delay_HP_Sprite_->SetSize(Vector2(static_cast<float>(Conversion(delay_HP_)), kHeight_));
	// リセット
	add_HP_ = 0;
	ImGui::Begin("HP");
	float now_HP_float = static_cast<float>(now_HP_);
	ImGui::SliderFloat("position", &now_HP_float, 0.0f, 1280.0f);
	now_HP_ = static_cast<uint32_t>(now_HP_float);
	ImGui::End();
}

void EnemyHP::Draw() { 
	delay_HP_Sprite_->Draw();
	now_HP_Sprite_->Draw();
}

float EnemyHP::Conversion(uint32_t num) { 	
	float hp_Max = static_cast<float>(kMax_HP_);
	float width = kWidth_;
	return static_cast<float>(num) * width / hp_Max;
}
