#pragma once
#include "Object3D.h"

/// <summary>
/// ‰æ–Ê‚ÉŠeíî•ñ‚ğ•\¦‚·‚éˆ—
/// </summary>
class DisplayInfo : public Object3D
{
public:
	DisplayInfo();
	~DisplayInfo();

	void Update() override;
	void Draw() override;
private:
	CSpriteImage* frameUiImage;
	CSprite* sprite;
};