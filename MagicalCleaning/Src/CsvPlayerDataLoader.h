#pragma once
#include "CsvLoadBase.h"

//定数定義
namespace {
    static const int MAX_PLAYER = 4;    //最大プレイヤー数
    static const int POWER_NUM = 10;    //能力レベルの段階数
}

// パラメータ構造体
struct PlayerParams {
    float InvisibleTime[MAX_PLAYER];
    float  DamageCoolTime;
    float TeleportTime;

    float MoveSpeedT[POWER_NUM];
    int   JumpNumT[POWER_NUM];
    float AtkSpeedT[POWER_NUM];
    float AtkRangeT[POWER_NUM];
    int   CarWeightT[POWER_NUM];

    int MoveSpeedC[POWER_NUM];
    int JumpNumC[POWER_NUM];
    int AtkSpeedC[POWER_NUM];
    int AtkRangeC[POWER_NUM];
    int CarWeightC[POWER_NUM];
};


// ---------------------------------------------------------------------------
// プレイ画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvPlayerDataLoader : public CsvLoadBase {
private:
    PlayerParams playerParams;

public:
    CsvPlayerDataLoader(const std::string& filePath);
    virtual ~CsvPlayerDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const PlayerParams& GetPlayerParams() const { return playerParams; }
};
