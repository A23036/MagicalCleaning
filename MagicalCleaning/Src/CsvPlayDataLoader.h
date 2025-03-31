#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// パラメータ構造体
struct PlayParams {
    int GameTime;
    int MaxLeafNum;
    int LeafSpawnRangeMin;
    int LeafSpawnRangeMax;
    int LeafSpawnHeightMin;
    int LeafSpawnHeightMax;
    VECTOR3 InitPos[4];
};


// ---------------------------------------------------------------------------
// プレイ画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvPlayDataLoader : public CsvLoadBase {
private:
    PlayParams playParams;

public:
    CsvPlayDataLoader(const std::string& filePath);
    virtual ~CsvPlayDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const PlayParams& GetPlayParams() const { return playParams; }
};
