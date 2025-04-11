#include "CsvPlayerDataLoader.h"

CsvPlayerDataLoader::CsvPlayerDataLoader(const std::string& filePath) : CsvLoadBase(filePath)
{
}

CsvPlayerDataLoader::~CsvPlayerDataLoader()
{
}

void CsvPlayerDataLoader::Load()
{
	for (int i = 1; i < csv.GetLines(); i++) { //CSVファイルから設定の読み込み
		if (csv.GetString(i, 0) == "Player") {
			std::string key = csv.GetString(i, 1);

            if (key == "InvisibleTime") {
                for (int j = 0; j < MAX_PLAYER; j++) {
                    playerParams.InvisibleTime[j] = csv.GetFloat(i, 3 + j);
                }
            }
            else if (key == "DamageCoolTime") {
                playerParams.DamageCoolTime = csv.GetFloat(i, 3);
            }
            else if (key == "TeleportTime") {
                playerParams.TeleportTime = csv.GetFloat(i, 3);
            }
            else if (key == "MoveSpeedT") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.MoveSpeedT[j] = csv.GetFloat(i, 3 + j);
                }
            }
            else if (key == "JumpNumT") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.JumpNumT[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "AtkSpeedT") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.AtkSpeedT[j] = csv.GetFloat(i, 3 + j);
                }
            }
            else if (key == "AtkRangeT") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.AtkRangeT[j] = csv.GetFloat(i, 3 + j);
                }
            }
            else if (key == "CarWeightT") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.CarWeightT[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "MoveSpeedC") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.MoveSpeedC[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "JumpNumC") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.JumpNumC[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "AtkSpeedC") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.AtkSpeedC[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "AtkRangeC") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.AtkRangeC[j] = csv.GetInt(i, 3 + j);
                }
            }
            else if (key == "CarWeightC") {
                for (int j = 0; j < POWER_NUM; j++) {
                    playerParams.CarWeightC[j] = csv.GetInt(i, 3 + j);
                }
            }
		}
		if (csv.GetString(i, 0) == "Leaf") { //Playerが終わったら読み込み終了
			break;
		}
	}
}
