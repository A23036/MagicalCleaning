#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// パラメータ構造体
struct CameraParams {
    float CHANGE_TIME;
    float ROTATE_SPEED;
    VECTOR3 CameraPos;
    VECTOR3 LookPos;
};


// ---------------------------------------------------------------------------
// プレイ画面で使用するデータのCSVファイル読み込み処理
// ---------------------------------------------------------------------------
class CsvCameraDataLoader : public CsvLoadBase {
private:
    CameraParams cameraParams;

public:
    CsvCameraDataLoader(const std::string& filePath);
    virtual ~CsvCameraDataLoader();
    void Load() override;

    // 読み込んだデータを取得
    const CameraParams& GetCameraParams() const { return cameraParams; }
};
