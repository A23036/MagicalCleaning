#include "Sky.h"

Sky::Sky()
{
	mesh = new CFbxMesh();
	mesh->Load("data/Map2/sky2.mesh");
	SetPosition(0, 0, 0);
	rotate = 0.0f;
	//GameDevice()->m_pD3D->SetZBuffer(false);
}

Sky::~Sky()
{
	SAFE_DELETE(mesh);
}

void Sky::Update()
{
	SetRotation(0, rotate * DegToRad, 0);
	rotate += 0.01;
}

void Sky::Draw()
{
	VECTOR3  prevVec = GameDevice()->m_vLightDir;
	GameDevice()->m_vLightDir = VECTOR3(0, 0, 0);
	Object3D::Draw();
	GameDevice()->m_vLightDir = prevVec;
}
