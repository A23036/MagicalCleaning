#pragma once
#include "CsvLoadBase.h"

//�萔��`
namespace {
    static const int MAX_PLAYER = 4;    //�ő�v���C���[��
    static const int POWER_NUM = 10;    //�\�̓��x���̒i�K��
}

// �p�����[�^�\����
struct PlayerParams {
    float InvisibleTime[MAX_PLAYER];
    float  DamageCoolTime;
    float TeleportTime;

    float MoveSpeedT[POWER_NUM];
    int   JumpNumT[POWER_NUM];
    float AtkSpeedT[POWER_NUM];
    float AtkRangeT[POWER_NUM];
    int   CarWeightT[POWER_NUM];

    int MoveSpeedC[POWER_NUM];
    int JumpNumC[POWER_NUM];
    int AtkSpeedC[POWER_NUM];
    int AtkRangeC[POWER_NUM];
    int CarWeightC[POWER_NUM];
};


// ---------------------------------------------------------------------------
// �v���C��ʂŎg�p����f�[�^��CSV�t�@�C���ǂݍ��ݏ���
// ---------------------------------------------------------------------------
class CsvPlayerDataLoader : public CsvLoadBase {
private:
    PlayerParams playerParams;

public:
    CsvPlayerDataLoader(const std::string& filePath);
    virtual ~CsvPlayerDataLoader();
    void Load() override;

    // �ǂݍ��񂾃f�[�^���擾
    const PlayerParams& GetPlayerParams() const { return playerParams; }
};
