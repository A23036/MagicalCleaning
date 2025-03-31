#pragma once
#include "CsvLoadBase.h"
#include "GameMain.h"

// �p�����[�^�\����
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
// �v���C��ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvPlayDataLoader : public CsvLoadBase {
private:
    PlayParams playParams;

public:
    CsvPlayDataLoader(const std::string& filePath);
    virtual ~CsvPlayDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const PlayParams& GetPlayParams() const { return playParams; }
};
