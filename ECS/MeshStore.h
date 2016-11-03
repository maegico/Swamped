#pragma once
#include "RenderingComponent.h"

struct MeshStore {
	Mesh m_m;
	DirectX::XMFLOAT3 m_bb[8];
};
