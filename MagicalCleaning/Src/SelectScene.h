#pragma once
#include "SceneBase.h"
#include "CsvSelectDataLoader.h"

//プロトタイプ宣言
class Player;
class SelectDisplay;
class CsvReader;

// ---------------------------------------------------------------------------
// セレクト画面処理
// ---------------------------------------------------------------------------
class SelectScene : public SceneBase
{
public:
	SelectScene();
	virtual ~SelectScene();
	void Init();
	void Update() override;
	void Draw() override;

	void CsvLoad(); //CSVファイルから読み込んだデータの設定を行う

private:
	//各種クラスのポインタ
	Player*			players[6];			//プレイヤーのリスト
	SelectDisplay*	sd;
	CsvReader*		csv;
	CSpriteImage*	selectBackImage;	//セレクト画面背景イメージ
	CSprite*		sprite;
	
	//CSVファイルから読み込んだデータを格納する構造体
	SelectParams selectParams;

	//CSVファイルから読み込まれるデータ
	float	ScrollSpeed;	//背景スクロール速度
	int		BackFrm;		//シーンを戻るのに必要な長押しフレーム

	//背景アニメーション用変数
	float	scrollX;		//背景画像位置X
	int		frm;			//経過フレーム

	//プレイヤー設定
	VECTOR3 initPos[6];		//キャラクター初期位置
};
