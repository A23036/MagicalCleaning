#pragma once
#include "Object3D.h"

class ScoreManager : public Object3D {
public:
	ScoreManager();
	~ScoreManager();
	void Update();

	int GetMp() { return mp; };
	void addMp(int n);
private:
	int mp;
	int weight;
};