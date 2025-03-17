#pragma once
#include "CsvReader.h"
#include <string>

// ---------------------------------------------------------------------------
// CSV�ǂݍ��݊��N���X ���A�s�[���|�C���g
// ---------------------------------------------------------------------------
class CsvLoadBase {
protected:
    CsvReader csv;

public:
    explicit CsvLoadBase(const std::string& filePath);
    virtual ~CsvLoadBase();

    //�ǂݍ��ݏ������������z�֐��� (�h���N���X�Ŏ������邽��)
    virtual void Load() = 0;
};
