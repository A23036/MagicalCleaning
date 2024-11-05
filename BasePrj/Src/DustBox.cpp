#include "DustBox.h"

DustBox::DustBox()
{
    SetTag("STAGEOBJ");
    mesh = new CFbxMesh();
    mesh->Load("data/models/chestA.mesh");
}

DustBox::~DustBox()
{
}

void DustBox::Update()
{
}

SphereCollider DustBox::Collider()
{
    SphereCollider col;
    col.center = transform.position + VECTOR3(0, 0.4f, 0);
    col.radius = 0.4f;
    return col;
}
