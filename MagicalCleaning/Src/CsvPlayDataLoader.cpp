#include "CsvPlayDataLoader.h"

CsvPlayDataLoader::CsvPlayDataLoader(const std::string& filePath) : CsvLoadBase(filePath)
{

}

CsvPlayDataLoader::~CsvPlayDataLoader()
{
}

void CsvPlayDataLoader::Load()
{
    for (int i = 1; i < csv.GetLines(); i++) {
        if (csv.GetString(i, 0) == "Play") {
            std::string key = csv.GetString(i, 1);

            if (key == "GameTime") {
                playParams.GameTime = csv.GetInt(i, 3);
            }
            else if (key == "MaxLeafNum") {
                playParams.MaxLeafNum = csv.GetInt(i, 3);
            }
            else if (key == "InitPos1") {
                playParams.InitPos[0].x = csv.GetInt(i, 3);
                playParams.InitPos[0].y = csv.GetInt(i, 4);
                playParams.InitPos[0].z = csv.GetInt(i, 5);
            }
            else if (key == "InitPos2") {
                playParams.InitPos[1].x = csv.GetInt(i, 3);
                playParams.InitPos[1].y = csv.GetInt(i, 4);
                playParams.InitPos[1].z = csv.GetInt(i, 5);
            }
            else if (key == "InitPos3") {
                playParams.InitPos[2].x = csv.GetInt(i, 3);
                playParams.InitPos[2].y = csv.GetInt(i, 4);
                playParams.InitPos[2].z = csv.GetInt(i, 5);
            }
            else if (key == "InitPos4") {
                playParams.InitPos[3].x = csv.GetInt(i, 3);
                playParams.InitPos[3].y = csv.GetInt(i, 4);
                playParams.InitPos[3].z = csv.GetInt(i, 5);
            }
            else if (key == "LeafSpawnRange") {
                playParams.LeafSpawnRangeMin = csv.GetInt(i, 3);
                playParams.LeafSpawnRangeMax = csv.GetInt(i, 4);
            }
            else if (key == "LeafSpawnHeight") {
                playParams.LeafSpawnHeightMin = csv.GetInt(i, 3);
                playParams.LeafSpawnHeightMax = csv.GetInt(i, 4);
            }
        }
        if (csv.GetString(i, 0) == "Result") { //Play‚ªI‚í‚Á‚½‚ç“Ç‚Ýž‚ÝI—¹
            break;
        }
    }
}