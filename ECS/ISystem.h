#pragma once
class Game { void RemoveEntity(unsigned int entity_id); };
class ISystem {
public:
	void virtual Update(Game * g) {};
	void virtual Create(unsigned int entityId) {};
	void virtual Remove(unsigned int entityId) {};
private:
};