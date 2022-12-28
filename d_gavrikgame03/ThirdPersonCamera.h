#pragma once

#include "Camera.h"
#include "Player.h"

class ThirdPersonCamera : public Camera
{
	float Velocity = 1.F;
	float Turn_velocity = 100.F;
	float Distance = 10.f;
	float y_Offset = 1.f;

	inline void updatePosition() {
		Position = player->position + Distance * glm::normalize(glm::vec3(-player->Front.x, y_Offset, -player->Front.z));
	}

	Player* player;

public:

	ThirdPersonCamera(Player* _player) : Camera(_player->position + glm::vec3(-5.0f, 5.0f, 0.0f), WORLD_UP, _player->rotation.y, -30.f)
	{
		player = _player;
		updatePosition();
	}

	~ThirdPersonCamera() {}

	virtual void ProcessMouse(glm::vec2 offset, GLboolean constrainPitch = true) override
	{
		Camera::ProcessMouse(glm::vec2(offset.x, 0.f), constrainPitch); // removing offset.y bcuz it looked strange.
		//Camera::ProcessMouse(offset, constrainPitch);
		if (player) player->SetYaw(-Yaw);
		updatePosition();
	}

	virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) override
	{
		float positionDelta = MovementSpeed * deltaTime;
		float turnDelta = Turn_velocity * deltaTime;

		if (direction == Camera::FORWARD) {
			if (player) player->SetPosition(positionDelta);
			updatePosition();
			return;
		}

		if (direction == Camera::LEFT) {
			Yaw -= turnDelta;
			if (player) player->SetYaw(-Yaw);
			updatePosition();
			updateVectors();
			return;
		}

		if (direction == Camera::BACKWARD) {
			if (player) player->SetPosition(-positionDelta);
			updatePosition();
			return;
		}

		if (direction == Camera::RIGHT) {
			Yaw += turnDelta;
			if (player) player->SetYaw(-Yaw);
			updatePosition();
			updateVectors();
			return;
		}
	}

};
