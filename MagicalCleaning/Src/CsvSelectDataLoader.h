#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// パラメータ構造体
struct SelectParams {
    float	ScrollSpeed;
    int		BackFrm;
};

struct SelectDisParams {
    int ColorPosY;
    int ColorIconSize;
    int ColorIconDispSize;
    int UiSpace;
    int GuideUiPosY;
    int MoveFrm;
    VECTOR2 BackUiPos;
};

// ---------------------------------------------------------------------------
// セレクト画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvSelectDataLoader : public CsvLoadBase {
private:
    SelectParams selectParams;
    SelectDisParams selectDisParams;

public:
    CsvSelectDataLoader(const std::string& filePath);
    virtual ~CsvSelectDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const SelectParams& GetSelectParams() const { return selectParams; }
    const SelectDisParams& GetSelectDisParams() const { return selectDisParams; }
};
