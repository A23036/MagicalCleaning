#pragma once
#include "Object3D.h"
#include "CsvSelectDataLoader.h"

//プロトタイプ宣言
class DataCarrier;
class EasingCalc;

// ---------------------------------------------------------------------------
// セレクト画面UI描画処理
// ---------------------------------------------------------------------------
class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	virtual ~SelectDisplay();

	void Init();
	void Update() override;
	void Draw() override;

	void DrawUI();		//プレイヤーの状態に応じたUIの描画
	void Transition();	//画面切り替え処理

	//選択状態に応じたスプライト描画処理
	void UpdateColorSelect(int playerNum,int ix,int iy);
	void UpdateCameraSetting(int playerNum,int ix,int iy);
	void UpdateReady(int playerNum);
	void UpdateEnd(int playerNum);

	void CsvLoad(); //CSVファイルから読み込んだデータの設定を行う

	//アクセス関数
	bool GetIsSelectColor(int playerNum) { return playerEntry[playerNum]; };
	bool GetIsSelected(int color) { return isSelect[color]; };
	int  GetSelectColor(int playerNum) { return selectColor[playerNum]; };
	bool GetIsReady() { return isReadyAll; };
	
private:
	//各種クラスのポインタ
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* selectUiImage;	//セレクト画面UIイメージ
	
	//キャラクターカラー
	enum Color {
		Red = 0,
		Blue,
		Yellow,
		Green,
		Purple,
		Black,
	};

	//設定状態
	enum SettingState {
		sColor,
		sCamera,
		sReady,
		sEnd,
	};

	SettingState state[MAXPLAYER]; //プレイヤーごとの設定状態保存用配列
	
	//CSVファイルから読み込んだデータを格納する構造体
	SelectDisParams selectDisParams;

	//CSVファイルから読み込まれるデータ
	int ColorPosY;			//キャラ選択カラー位置Y
	int ColorIconSize;		//キャラ選択カラーサイズ
	int ColorIconDispSize;	//キャラ選択カラー表示サイズ
	int UiSpace;			//UI同士の間隔
	int GuideUiPosY;		//操作方法ガイドUI位置Y
	int MoveFrm;			//カーソルが動くのに必要な入力フレーム量
	VECTOR2 BackUiPos;		//戻るUI位置


	bool isReadyAll;				//プレイヤー全員が準備完了
	bool playerEntry[MAXPLAYER];	//プレイヤーのエントリー状態
	bool isReady[MAXPLAYER];		//プレイヤー準備完了判定
	bool isSetUpCamera[MAXPLAYER];	//プレイヤーごとのカメラ設定完了判定
	bool isSelect[6];				//カラーごとの選択済み判定
	bool isTransition;				//トランジション中判定
	bool isFirst;					//初回トランジション判定
	int  selectColor[MAXPLAYER];	//プレイヤーごとの選択中カラー
	int  selectCamera[MAXPLAYER];	//プレイヤーごとの選択中カメラ設定 0:オート/1:マニュアル
	int moveFrm[MAXPLAYER];			//プレイヤーごとの入力フレームカウント用変数
	int animFrm[MAXPLAYER];			//プレイヤーごとの準備完了アニメーション経過フレーム保存
	int  transFrm;					//トランジションフレーム
	VECTOR2 cursorPos[MAXPLAYER];	//カーソル位置
};