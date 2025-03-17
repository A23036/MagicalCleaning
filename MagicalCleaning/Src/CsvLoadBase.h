#pragma once
#include "CsvReader.h"
#include <string>

// ---------------------------------------------------------------------------
// CSV読み込み基底クラス ※アピールポイント
// ---------------------------------------------------------------------------
class CsvLoadBase {
protected:
    CsvReader csv;

public:
    explicit CsvLoadBase(const std::string& filePath);
    virtual ~CsvLoadBase();

    //読み込み処理を純粋仮想関数に (派生クラスで実装するため)
    virtual void Load() = 0;
};
