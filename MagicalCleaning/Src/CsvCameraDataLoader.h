#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// �p�����[�^�\����
struct CameraParams {
    float CHANGE_TIME;
    float ROTATE_SPEED;
    VECTOR3 CameraPos;
    VECTOR3 LookPos;
};


// ---------------------------------------------------------------------------
// �v���C��ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvCameraDataLoader : public CsvLoadBase {
private:
    CameraParams cameraParams;

public:
    CsvCameraDataLoader(const std::string& filePath);
    virtual ~CsvCameraDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const CameraParams& GetCameraParams() const { return cameraParams; }
};
