#pragma once
//#include "IGame.h"
class Game;
class ISystem {
public:
	void virtual Update(Game * g) {};
	void virtual Create(unsigned int entityId) {};
	void virtual Remove(unsigned int entityId) {};
private:
};