#pragma once

#include "WalkMesh.hpp"
#include "Scene.hpp"

struct Gun {
	
	Gun(Scene::Transform* player_transform, Scene::Transform* fire_point, int16_t max_ammo, float muzzle_velocity, float fire_rate_delay, float reload_time);
	Gun() = default;
	~Gun();

	void UpdateTimer(float elapsed); // call every frame with elapsed seconds
	bool Shoot(glm::vec3 dir); // returns if bullet was actually shot

	// Transform of bullet spawning location
	Scene::Transform* fire_point = nullptr;
	int16_t cur_ammo = 24;
	int16_t max_ammo = 24;
	float fire_rate_delay = 0.25f; // seconds between shots
	float reload_time = 2.5f;
	float muzzle_velocity = 10.0f;

	enum GunState {
		shooting, reloading, idle
	};

	GunState cur_state = idle;

private:
	float internal_timer = fire_rate_delay; // used for fire rate; gun shoots on 0
	bool shooting_prev = false; // true if player was shooting this gun last frame (holding shoot button)
};

class Player {

public:
	WalkPoint at;
	// Transform of the player mesh in the scene; will be yawed by mouse left/right motion:
	Scene::Transform* transform = nullptr;
	// Camera in scene, at head of player; will be pitched by mouse up/down motion:
	Scene::Camera* camera = nullptr;

	Gun cur_gun;

private:

};

