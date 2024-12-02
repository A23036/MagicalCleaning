#pragma once

//イージング関数をまとめておくクラス
class EasingCalc
{
public:
	float easeOutBack(float x);		//最後に少し飛び出してから戻る
	float easeOutBounce(float x);	//最後にはねる
};