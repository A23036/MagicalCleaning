#pragma once
#include "CsvLoadBase.h"

// パラメータ構造体
struct TitleParams {
    float AnimTime[4];
    float ScaleStart;
    float ScaleGoal;
    float RotateStart;
    float RotateGoal;
    float TransStart;
    float TransGoal;
    float AlphaStart;
    float AlphaGoal;
};

// ---------------------------------------------------------------------------
// タイトル画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvTitleDataLoader : public CsvLoadBase {
private:
    TitleParams titleParams;

public:
    CsvTitleDataLoader(const std::string& filePath);
    virtual ~CsvTitleDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const TitleParams& GetTitleParams() const { return titleParams; }
};
