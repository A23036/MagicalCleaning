#pragma once
#include <string>
#include "Object3D.h"

class Particle : public Object3D {
public:
	Particle();
	~Particle();
	void Draw() override;

	void LoadTexture(std::string filename, const VECTOR2& uvSize);
	class Chip {
	public:
		VECTOR3 offset;
		float scale;
		VECTOR2 uv;
		float alpha;
		float order; //ÉJÉÅÉâÇ∆ÇÃãóó£
		Chip() {
			offset = VECTOR3(0, 0, 0);
			scale = 1.0f;
			uv = VECTOR2(0, 0);
			alpha = 1.0f;
		}
	};

protected:
	bool useAddBlend;
	std::list<Chip> chips;
private:
	ID3D11ShaderResourceView* texture;
	ID3D11Buffer* vertexBuffer;
	VECTOR2 uvSize;
};