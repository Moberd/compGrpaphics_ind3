#pragma once
#include "Entity.h"
//#include "Player.h"
//#include "Battlefield.h"
class Missile : public Entity
{
	//friend class Player;
	//friend class Battlefield;

	float velocity = 30.f;

public:

	glm::vec3 Front;

	bool isHidden = true; // indicates if bomb has been planted.

	inline void Move(const float deltaTime) { position += deltaTime * velocity * Front; }

	Missile(const Mesh* m, const Shader* s) : Entity(m, s) {}

	~Missile() {}
};