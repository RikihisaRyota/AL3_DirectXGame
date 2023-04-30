#include "TimeCall.h"

TimeCall::TimeCall(std::function<void(void)> callBack, uint32_t time)
    : callBack_(callBack), time_(time) {}

void TimeCall::Update() {
	if (isDead_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isDead_ = true;
		//コールバック関数呼び出し
		callBack_();
	}
}