#pragma once
#include "Object3D.h"

/// <summary>
/// 画面に各種情報を表示する処理
/// </summary>
class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	~SelectDisplay();

	void Update() override;
	void Draw() override;
private:
	CSprite* sprite;
	CSpriteImage* selectUiImage;	//セレクト画面UIイメージ
	
	int ColorPosY;			//キャラ選択カラー位置Y
	int ColorIconSize;		//キャラ選択カラーサイズ
	int ColorIconDispSize;	//キャラ選択カラー表示サイズ
	int UiSpace;			//UI同士の間隔
	int GuideUiPosY;		//操作方法ガイドUI位置Y
	VECTOR2 BackUiPos;		//戻るUI位置

	bool playerEntry[4];	//プレイヤーのエントリー状態
	int selectColor[4];		//プレイヤーごとの選択中カラー
	VECTOR2 cursorPos[4];	//カーソル位置
	
	enum Dir{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};

	enum Color {
		Red = 0,
		Blue,
		Yellow,
		Green,
		Purple,
		Black,
	};
};