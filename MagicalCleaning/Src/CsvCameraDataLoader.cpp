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

			if (key == "CameraPos") {		// �J�������W
				cameraParams.CameraPos = VECTOR3(csv.GetFloat(i, 3), csv.GetFloat(i, 4), csv.GetFloat(i, 5));
			}
			if (key == "LookPos") {		// �J���������_
				cameraParams.LookPos = VECTOR3(csv.GetFloat(i, 3), csv.GetFloat(i, 4), csv.GetFloat(i, 5));
			}
			if (key == "ChangeTime") {		// �J�����؂�ւ�����
				cameraParams.CHANGE_TIME = csv.GetFloat(i, 3);
			}
			if (key == "RotateSpeed") {	// �J������]���x
				cameraParams.ROTATE_SPEED = csv.GetFloat(i, 3);
			}
        }
        if (csv.GetString(i, 0) == "Player") { //Camera���I�������ǂݍ��ݏI��
            break;
        }
    }
}
