#include "CsvSelectDataLoader.h"

CsvSelectDataLoader::CsvSelectDataLoader(const std::string& filePath) : CsvLoadBase(filePath)
{

}

CsvSelectDataLoader::~CsvSelectDataLoader()
{
}

void CsvSelectDataLoader::Load()
{
    for (int i = 1; i < csv.GetLines(); i++) {
        if (csv.GetString(i, 0) == "Select") {
            std::string key = csv.GetString(i, 1);

            if (key == "ScrollSpeed") {
                selectParams.ScrollSpeed = csv.GetFloat(i, 3);
            }
            else if (key == "BackFrm") {
                selectParams.BackFrm = csv.GetFloat(i, 3);
            }
            else if (key == "ColorPosY") {
                selectDisParams.ColorPosY = csv.GetInt(i, 3);
            }
            else if (key == "ColorIconSize") {
                selectDisParams.ColorIconSize = csv.GetInt(i, 3);
            }
            else if (key == "ColorIconDispSize") {
                selectDisParams.ColorIconDispSize = csv.GetInt(i, 3);
            }
            else if (key == "UiSpace") {
                selectDisParams.UiSpace = csv.GetInt(i, 3);
            }
            else if (key == "GuideUiPosY") {
                selectDisParams.GuideUiPosY = csv.GetInt(i, 3);
            }
            else if (key == "MoveFrm") {
                selectDisParams.MoveFrm = csv.GetInt(i, 3);
            }
            else if (key == "BackUiPos") {
                selectDisParams.BackUiPos.x = csv.GetInt(i, 3);
                selectDisParams.BackUiPos.y = csv.GetInt(i, 4);
            }
        }
        if (csv.GetString(i, 0) == "Play") { //Select‚ªI‚í‚Á‚½‚ç“Ç‚Ýž‚ÝI—¹
            break;
        }
    }
}