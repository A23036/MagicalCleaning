#include "EasingCalc.h"
#include "GameMain.h"

float EasingCalc::easeOutBack(float x) //ç≈å„Ç…è≠ÇµîÚÇ—èoÇµÇƒÇ©ÇÁñﬂÇÈ
{
	float c1 = 1.70158;
	float c3 = c1 + 1;

	return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

float EasingCalc::easeOutBounce(float x) //ç≈å„Ç…ÇÕÇÀÇÈ
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