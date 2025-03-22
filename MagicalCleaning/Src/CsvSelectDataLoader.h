#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// �p�����[�^�\����
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
// �Z���N�g��ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvSelectDataLoader : public CsvLoadBase {
private:
    SelectParams selectParams;
    SelectDisParams selectDisParams;

public:
    CsvSelectDataLoader(const std::string& filePath);
    virtual ~CsvSelectDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const SelectParams& GetSelectParams() const { return selectParams; }
    const SelectDisParams& GetSelectDisParams() const { return selectDisParams; }
};
