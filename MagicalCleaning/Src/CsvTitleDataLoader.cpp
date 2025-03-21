#include "CsvTitleDataLoader.h"
#include "GameMain.h"

//
CsvTitleDataLoader::CsvTitleDataLoader(const std::string& filePath) : CsvLoadBase(filePath) 
{

}

CsvTitleDataLoader::~CsvTitleDataLoader()
{
}

void CsvTitleDataLoader::Load() {
    for (int i = 1; i < csv.GetLines(); i++) {
        if (csv.GetString(i, 0) == "Title") {
            std::string key = csv.GetString(i, 1);

            if (key == "AnimTime") {
                titleParams.AnimTime[0] = csv.GetFloat(i, 3);
                titleParams.AnimTime[1] = csv.GetFloat(i, 4);
                titleParams.AnimTime[2] = csv.GetFloat(i, 5);
                titleParams.AnimTime[3] = csv.GetFloat(i, 6);
            }
            else if (key == "ScaleStart") {
                titleParams.ScaleStart = csv.GetFloat(i, 3);
            }
            else if (key == "ScaleGoal") {
                titleParams.ScaleGoal = csv.GetFloat(i, 3);
            }
            else if (key == "RotateStart") {
                titleParams.RotateStart = csv.GetFloat(i, 3) * DegToRad;
            }
            else if (key == "RotateGoal") {
                titleParams.RotateGoal = csv.GetFloat(i, 3) * DegToRad;
            }
            else if (key == "TransStart") {
                titleParams.TransStart = csv.GetFloat(i, 3);
            }
            else if (key == "TransGoal") {
                titleParams.TransGoal = csv.GetFloat(i, 3);
            }
            else if (key == "AlphaStart") {
                titleParams.AlphaStart = csv.GetFloat(i, 3);
            }
            else if (key == "AlphaGoal") {
                titleParams.AlphaGoal = csv.GetFloat(i, 3);
            }
        }
        if (csv.GetString(i, 0) == "Select") { //Title‚ªI‚í‚Á‚½‚ç“Ç‚Ýž‚ÝI—¹
            break;
        }
    }
}
