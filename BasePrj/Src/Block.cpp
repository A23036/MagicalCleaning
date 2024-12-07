#include "Block.h"

Block::Block(int num, VECTOR3 pos)
{
	mesh = new CFbxMesh();
	meshCol = new CFbxMesh();
	blockCol = new CCollision;

	switch (num) {
	case Block1:
		mesh->Load("data/Map2/MapChip/1_1Block.mesh");
		meshCol->Load("data/Map2/MapChip/1_1BlockCol.mesh");
		blockCol->AddFbxLoad(meshCol,pos);
		break;
	case Block2:
		mesh->Load("data/Map2/MapChip/1_2Block.mesh");
		meshCol->Load("data/Map2/MapChip/1_2BlockCol.mesh");
		blockCol->AddFbxLoad(meshCol, pos);
		break;
	}

	this->SetPosition(pos); //”z’u

}

Block::~Block()
{
}

void Block::Start()
{
}

void Block::Update()
{
}
