#include "Renderer.h"



Renderer::Renderer()
{
}

Renderer::Renderer(ID3D11DeviceContext* context, std::vector<Entity*>* entities) : context(context)
{
	this->entities = std::vector<Entity*>(entities->size());
	this->visual = std::vector<bool>(entities->size());
	memcpy(&(this->entities), entities, sizeof(&entities));
}


Renderer::~Renderer()
{
}

void Renderer::draw()
{
}