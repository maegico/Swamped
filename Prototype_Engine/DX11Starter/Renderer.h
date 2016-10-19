#pragma once
#include "Entity.h"
#include <vector>

//Will work on this later

class Renderer
{
public:
	Renderer();
	Renderer(ID3D11DeviceContext* devCon, std::vector<Entity*>* entities);	//might not want the vector pointer
	~Renderer();

	void draw();

private:
	std::vector<Entity*> entities;
	//std::vector<Mesh*> meshes;	??
	std::vector<bool> visual;
	ID3D11DeviceContext* context;
};

