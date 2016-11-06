#pragma once
#include "GameEntity.h"
#include "Camera.h"
#include <array>

class Ghost {

public: 
	Ghost(GameEntity* ghost, DirectX::XMFLOAT3 playerStart);
	Ghost(); 
	~Ghost();
	GameEntity* ghostEntity; 
	DirectX::XMFLOAT3 playerPos;
	bool phasing;	//is the ghost phasing through something
	float PHASINGSPEED;		//ghost moves slower while phasing
	float NORMALSPEED; 
	float maxSpeed;		//use to cap acceleration
	GameEntity* charms[5];	//the charms that ward the ghost off

	//These variables are for steering behaviors
	DirectX::XMFLOAT3 velocity; 
	DirectX::XMFLOAT3 acceleration; 
	DirectX::XMFLOAT3 force; 

	void Update(DirectX::XMFLOAT3 playerPosition);
	void Seek(DirectX::XMFLOAT3 playerPosition);
	void ObstacleAvoid(); 
};