#pragma once
class Game; //we need this to avoid circular dependencies
class ISystem {
public:
	void virtual Update(Game * g) {};
	void virtual Create(unsigned int entityId) {};
	void virtual Remove(unsigned int entityId) {};
private:
};