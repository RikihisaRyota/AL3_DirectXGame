#pragma once
#include <functional>
/// <summary>
/// 時限発動
/// </summary>
class TimeCall {
public:
	// コンストラクタ
	TimeCall(std::function<void(void)> callBack, uint32_t time);
	// 更新
	void Update();
	// 完了ならtrueを返す
	bool IsDead() { return isDead_; }

private:
	// コールバック
	std::function<void(void)> callBack_;
	// 残り時間
	uint32_t time_;
	// 完了フラグ
	bool isDead_ = false;
};