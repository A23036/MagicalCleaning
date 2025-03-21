#include "EasingCalc.h"
#include "GameMain.h"

float EasingCalc::easeOutBack(float x) //最後に少し飛び出してから戻る
{
	float c1 = 1.70158;
	float c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

float EasingCalc::easeOutBounce(float x) //最後にはねる
{
	float n1 = 7.5625f;
	float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	}
	else if (x < 2.5 / d1) {
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	}
	else {
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}

float EasingCalc::easeOutExpo(float x) //急加速から減速
{
	if (x == 1.0f) {
		return 1.0f;
	}
	else {
		return 1.0f - pow(2.0f, -10.0f * x);
	}
}
