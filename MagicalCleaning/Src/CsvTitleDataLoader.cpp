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
                animParams.AnimTime[0] = csv.GetFloat(i, 3);
                animParams.AnimTime[1] = csv.GetFloat(i, 4);
                animParams.AnimTime[2] = csv.GetFloat(i, 5);
                animParams.AnimTime[3] = csv.GetFloat(i, 6);
            }
            else if (key == "ScaleStart") {
                animParams.ScaleStart = csv.GetFloat(i, 3);
            }
            else if (key == "ScaleGoal") {
                animParams.ScaleGoal = csv.GetFloat(i, 3);
            }
            else if (key == "RotateStart") {
                animParams.RotateStart = csv.GetFloat(i, 3) * DegToRad;
            }
            else if (key == "RotateGoal") {
                animParams.RotateGoal = csv.GetFloat(i, 3) * DegToRad;
            }
            else if (key == "TransStart") {
                animParams.TransStart = csv.GetFloat(i, 3);
            }
            else if (key == "TransGoal") {
                animParams.TransGoal = csv.GetFloat(i, 3);
            }
            else if (key == "AlphaStart") {
                animParams.AlphaStart = csv.GetFloat(i, 3);
            }
            else if (key == "AlphaGoal") {
                animParams.AlphaGoal = csv.GetFloat(i, 3);
            }
        }
        if (csv.GetString(i, 0) == "Select") { //Title‚ªI‚í‚Á‚½‚ç“Ç‚Ýž‚ÝI—¹
            break;
        }
    }
}
