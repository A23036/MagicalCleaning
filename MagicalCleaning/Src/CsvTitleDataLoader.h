#pragma once
#include "CsvLoadBase.h"

// �p�����[�^�\����
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
// �^�C�g����ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvTitleDataLoader : public CsvLoadBase {
private:
    TitleParams titleParams;

public:
    CsvTitleDataLoader(const std::string& filePath);
    virtual ~CsvTitleDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const TitleParams& GetTitleParams() const { return titleParams; }
};
