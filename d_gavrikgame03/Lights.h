#pragma once

#include "headers.h"
#include "Shader.h"

struct DirLight {
	glm::vec3 direction{ 0.f, -1.f, -1.f };
	glm::vec4 ambient{ 0.2f, 0.2f, 0.2f, 1.f };
	glm::vec4 diffuse{ 0.8f, 0.8f, 0.8f, 1.f };
	glm::vec4 specular{ 1.0f, 1.0f, 1.0f, 1.f };

	void SetUniforms(const Shader* s) {
		s->use();

		s->SetVec3("dirLight.direction", direction);
		s->SetVec4("dirLight.ambient", ambient);
		s->SetVec4("dirLight.diffuse", diffuse);
		s->SetVec4("dirLight.specular", specular);

		glUseProgram(0);
	}
};

struct SpotLight {

	glm::vec3 position{ 0.f };
	glm::vec3 direction{ 1.f, 0.f, 0.f };

	glm::vec4 ambient{ 1.0f, 0.2f, 0.2f, 1.f };
	glm::vec4 diffuse{ 1.0f, 0.0f, 0.0f, 1.f };
	glm::vec4 specular{ 1.0f, 1.0f, 1.0f, 1.f };

	glm::vec3 attenuation{ 0.6f, 0.00007f, 0.0000014f };

	float cutOff = glm::cos(glm::radians(10.f));
	float exponent = 190.f;

	float intensity = 1.f;

	void SetUniforms(const Shader* s) {
		s->use();

		s->SetVec3("spotLight.position", position);
		s->SetVec3("spotLight.direction", direction);

		s->SetVec4("spotLight.ambient", ambient);
		s->SetVec4("spotLight.diffuse", diffuse);
		s->SetVec4("spotLight.specular", specular);

		s->SetVec3("spotLight.attenuation", attenuation);

		s->SetFloat("spotLight.cutOff", cutOff);
		s->SetFloat("spotLight.exponent", exponent);
		s->SetFloat("spotLight.intensity", intensity);

		glUseProgram(0);
	}
};