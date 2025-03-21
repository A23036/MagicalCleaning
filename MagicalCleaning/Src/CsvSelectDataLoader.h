#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// �p�����[�^�\����
struct SelectParams {
    float	ScrollSpeed;
    int		BackFrm;
};

// ---------------------------------------------------------------------------
// �Z���N�g��ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvSelectDataLoader : public CsvLoadBase {
private:
    SelectParams selectParams;

public:
    CsvSelectDataLoader(const std::string& filePath);
    virtual ~CsvSelectDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const SelectParams& GetSelectParams() const { return selectParams; }
};
