#pragma once
#include <tuple>
#include <random>
#include "Scene.h"
#include "Missile.h"
#include "ThirdPersonCamera.h"

class Battlefield : public Scene
{
	size_t enemy_copies = 10;
	size_t decor_copies = 10;

public:

	Player* player;
	std::vector<Entity> enemies;

	Battlefield(const std::vector<std::pair<const char*, const char*>> shader_paths,
		std::pair<const char*, const char*> player_obj_tex,
		std::pair<const char*, const char*> field_obj_tex,
		const std::vector<std::tuple<const char*, const char*, glm::vec3>> enemies_paths,
		const std::vector<std::pair<const char*, const char*>> decor_paths,
		std::tuple<const char*, const char*, glm::vec3> missile_path)
	{
		for (auto& vert_frag : shader_paths) {
			shaders.push_back(Shader(vert_frag.first, vert_frag.second));
		}

		meshes.push_back(Mesh(player_obj_tex.first));
		materials.push_back(Material(player_obj_tex.second));

		meshes.push_back(Mesh(field_obj_tex.first));
		materials.push_back(Material(field_obj_tex.second));

		for (auto& enemy_obj_tex : enemies_paths) {
			meshes.push_back(Mesh(std::get<0>(enemy_obj_tex)));
			materials.push_back(Material(std::get<1>(enemy_obj_tex)));
		}

		for (auto& env_obj_tex : decor_paths) {
			meshes.push_back(Mesh(env_obj_tex.first));
			materials.push_back(Material(env_obj_tex.second));
		}

		meshes.push_back(Mesh(std::get<0>(missile_path)));
		materials.push_back(Material(std::get<1>(missile_path)));

		for (size_t i = 0; i < meshes.size(); i++) {
			meshes[i].material = &materials[i];
		}

		player = new Player(&meshes[0], &shaders[0]);
		camera = new ThirdPersonCamera(player);

		// filling the teapot. with water.
		materials[materials.size() - 1].emission = glm::vec4(1.f);
		materials[materials.size() - 1].specular = glm::vec4(1.f);
		materials[materials.size() - 1].shininess = 55.f;
		for (size_t i = 0; i < player->missiles_count; i++) {
			player->missiles.push_back(Missile(&meshes[meshes.size() - 1], &shaders[0]));
			player->missiles[player->missiles.size() - 1].scale = std::get<2>(missile_path);
		}

		auto field = Entity(&meshes[1], &shaders[0]);
		entities.push_back(field);

		size_t offset = 2; // first two meshes are "tank" and field, 
		// so its our offset when were looking for meshes for enemies and decorations

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-20.f, 20.f); // here we set enemies & decor positions on the field

		for (size_t i = 0; i < enemies_paths.size(); i++) {
			for (size_t copy = 0; copy < enemy_copies; copy++) {
				auto newEnemy = Entity(&meshes[offset + i], &shaders[0]);
				newEnemy.scale = std::get<2>(enemies_paths[i]);
				newEnemy.position = glm::vec3(dis(gen), 0.01f, dis(gen));
				enemies.push_back(newEnemy);
			}
		}

		offset += enemies_paths.size();
		for (size_t i = 0; i < decor_paths.size(); i++) {
			for (size_t copy = 0; copy < decor_copies; copy++) {
				auto newDecoraion = Entity(&meshes[offset + i], &shaders[0]);
				newDecoraion.position = glm::vec3(dis(gen), 0.f, dis(gen));
				entities.push_back(newDecoraion);
			}
		}

		// lights.

		lights.push_back(DirLight());
		for (auto& s : shaders) {
			lights[0].SetUniforms(&s);
		}
	}

	~Battlefield() {}

	virtual void Draw() override {
		for (auto& s : shaders) {
			camera->UpdateUniforms(&s);
			player->headlight.SetUniforms(&s);
		}

		player->Draw();

		for (auto& ent : entities) {
			ent.Draw();
		}

		ApproachTheStorm();

		for (auto& ent : enemies) {
			ent.Draw();
		}

		for (auto& m : player->missiles) {
			if (m.isHidden) continue;
			m.Move(deltaTime);
			m.Draw();
		}
	}

	void ApproachTheStorm() { // checking if some missiles have hit some enemies
		for (auto& m : player->missiles) {
			if (m.isHidden) continue;
			if (glm::length(m.position - entities[0].position) >= 25.f) {
				m.isHidden = true;
				continue;
			} // checking distance from center of field. if too long - return missile into mag

			auto it = std::find_if(enemies.begin(), enemies.end(), [&](auto& enemy) { return glm::length(enemy.position - m.position) <= enemy.GetColliderR() + m.GetColliderR(); });

			if (it == enemies.end()) continue;

			m.isHidden = true;
			enemies.erase(it);
		}
	}

private:

};
