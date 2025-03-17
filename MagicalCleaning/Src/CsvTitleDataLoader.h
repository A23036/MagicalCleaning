#pragma once
#include "CsvLoadBase.h"

// �A�j���[�V�����p�����[�^�\����
struct AnimParams {
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
    AnimParams animParams;

public:
    CsvTitleDataLoader(const std::string& filePath);
    virtual ~CsvTitleDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const AnimParams& GetAnimParams() const { return animParams; }
};
