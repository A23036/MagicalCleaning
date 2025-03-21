#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// パラメータ構造体
struct SelectParams {
    float	ScrollSpeed;
    int		BackFrm;
};

// ---------------------------------------------------------------------------
// セレクト画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvSelectDataLoader : public CsvLoadBase {
private:
    SelectParams selectParams;

public:
    CsvSelectDataLoader(const std::string& filePath);
    virtual ~CsvSelectDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const SelectParams& GetSelectParams() const { return selectParams; }
};
