#include "CsvCameraDataLoader.h"

CsvCameraDataLoader::CsvCameraDataLoader(const std::string& filePath) : CsvLoadBase(filePath)
{
}

CsvCameraDataLoader::~CsvCameraDataLoader()
{
}

void CsvCameraDataLoader::Load()
{
    for (int i = 1; i < csv.GetLines(); i++) {
        if (csv.GetString(i, 0) == "Camera") {
            std::string key = csv.GetString(i, 1);

			if (key == "CameraPos") {		// カメラ座標
				cameraParams.CameraPos = VECTOR3(csv.GetFloat(i, 3), csv.GetFloat(i, 4), csv.GetFloat(i, 5));
			}
			if (key == "LookPos") {		// カメラ注視点
				cameraParams.LookPos = VECTOR3(csv.GetFloat(i, 3), csv.GetFloat(i, 4), csv.GetFloat(i, 5));
			}
			if (key == "ChangeTime") {		// カメラ切り替え時間
				cameraParams.CHANGE_TIME = csv.GetFloat(i, 3);
			}
			if (key == "RotateSpeed") {	// カメラ回転速度
				cameraParams.ROTATE_SPEED = csv.GetFloat(i, 3);
			}
        }
        if (csv.GetString(i, 0) == "Player") { //Cameraが終わったら読み込み終了
            break;
        }
    }
}
