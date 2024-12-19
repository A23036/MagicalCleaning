#pragma once
#include "Object3D.h"
#include "DataCarrier.h"

/// <summary>
/// 画面に各種情報を表示する処理
/// </summary>

enum Color {
	Red = 0,
	Blue,
	Yellow,
	Green,
	Purple,
	Black,
};

class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	~SelectDisplay();

	void Update() override;
	void Draw() override;

	void DrawUI();

	bool GetIsSelectColor(int playerNum) { return playerEntry[playerNum]; };
	bool GetIsSelected(int color) { return isSelect[color]; };
	int GetSelectColor(int playerNum) { return selectColor[playerNum]; };
	bool GetIsReady() { return isReadyAll; };
	
private:
	DataCarrier* dc;
	CSprite* sprite;
	CSpriteImage* selectUiImage;	//セレクト画面UIイメージ
	
	int ColorPosY;			//キャラ選択カラー位置Y
	int ColorIconSize;		//キャラ選択カラーサイズ
	int ColorIconDispSize;	//キャラ選択カラー表示サイズ
	int UiSpace;			//UI同士の間隔
	int GuideUiPosY;		//操作方法ガイドUI位置Y
	VECTOR2 BackUiPos;		//戻るUI位置

	bool isReadyAll;				//プレイヤー全員が準備完了
	bool playerEntry[MAXPLAYER];	//プレイヤーのエントリー状態
	bool isReady[MAXPLAYER];		//プレイヤー準備完了判定
	int  selectColor[MAXPLAYER];	//プレイヤーごとの選択中カラー
	int  selectCamera[MAXPLAYER];	//プレイヤーごとの選択中カメラ設定 0:オート/1:マニュアル
	bool isSetUpCamera[MAXPLAYER];	//プレイヤーごとのカメラ設定完了判定
	bool isSelect[6];				//カラーごとの選択済み判定
	VECTOR2 cursorPos[MAXPLAYER];	//カーソル位置

	int moveFrm[MAXPLAYER];	//プレイヤーごとの入力フレームカウント用変数
	int MoveFrm;			//カーソルが動くのに必要な入力フレーム量
};