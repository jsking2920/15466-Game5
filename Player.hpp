#pragma once

#include "WalkMesh.hpp"
#include "Scene.hpp"

struct Gun {
	
	Gun(Scene::Transform* player_transform, Scene::Transform* _fire_point, int16_t _max_ammo, float _muzzle_velocity, float _fire_rate_delay, float _reload_time);
	Gun() = default;
	~Gun();

	void UpdateTimer(float elapsed, bool shoot_button_held); // call every frame with elapsed seconds
	bool Shoot(glm::vec3 dir); // returns if bullet was actually shot
	bool Reload(); // returns false if gun fails to reload (only happens if it was already fully loaded or is already being reloaded)


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
	float reload_timer = reload_time; // used for reloading
	//bool shooting_prev = false; // true if player was shooting this gun last frame (holding shoot button)
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

