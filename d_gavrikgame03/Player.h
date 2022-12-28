#pragma once

#include "ThirdPersonCamera.h"
#include "Missile.h"
#include "Battlefield.h"
#include "Lights.h"

class Player : public Entity
{
	void updateVectors() {
		glm::vec3 front;
		front.x = cos(rotation.y) * cos(rotation.x);
		front.y = sin(rotation.x);
		front.z = -sin(rotation.y) * cos(rotation.x);
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp)); // these are not neccessary bcuz teapot is only sliding at xz plane
		Up = glm::normalize(glm::cross(Right, Front));

		headlight.direction = Front;
		headlight.position = position + NosePosition();
	}

	size_t missiles_count = 10;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp{ 0.f, 1.f, 0.f };

	inline glm::vec3 NosePosition() { return Up + Front * 1.2f; }

protected:

	SpotLight headlight;

	std::vector<Missile> missiles;

	friend class Battlefield;
	friend class ThirdPersonCamera;

	inline void SetPosition(const float positionDelta) {
		position += positionDelta * Front;
		headlight.position = position + NosePosition();
	}

	inline void SetYaw(const float yaw) {
		rotation.y = glm::radians(yaw);
		updateVectors();
	}

public:

	Player(const Mesh* m, const Shader* s) : Entity(m, s) {
		updateVectors();
	}

	~Player() {}

	void DropMissile() {
		auto it = std::find_if(missiles.begin(), missiles.end(), [](auto& m) { return m.isHidden; });
		if (it == missiles.end()) return;
		(*it).isHidden = false;
		(*it).position = position + NosePosition();
		(*it).Front = Front;
	}

	inline void SwitchHeadlight() { headlight.intensity = !headlight.intensity; }
	inline void ChangeExp(const float delta) {
		auto newExp = headlight.exponent + delta;
		if (newExp <= 0.f) return;
		headlight.exponent = newExp;
		//printf("%lf\n", newExp);
	}

	inline void ChangeCutOff(const float delta) {
		auto newVal = headlight.cutOff + delta;
		if ((newVal >= 1.f) || (newVal <= -1.f)) return;
		headlight.cutOff = newVal;
		//printf("%lf\n", newVal);
	}
};