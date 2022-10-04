#include "Player.hpp"

#include <glm/glm.hpp>
#include <iostream>

Gun::~Gun() {
}

Gun::Gun(Scene::Transform* player_transform, Scene::Transform* _fire_point, int16_t _max_ammo, float _muzzle_velocity, float _fire_rate_delay, float _reload_time) {
	
	fire_point = _fire_point;
	fire_point->parent = player_transform;

	reload_time = _reload_time;
	max_ammo = _max_ammo;
	cur_ammo = _max_ammo;
	fire_rate_delay = _fire_rate_delay;
	internal_timer = _fire_rate_delay;
	muzzle_velocity = _muzzle_velocity;
}

void Gun::UpdateTimer(float elapsed, bool shoot_button_held) {

	if (cur_state == reloading) {
		reload_timer -= elapsed;

		if (reload_timer <= 0.0f) {
			cur_state = idle;
			cur_ammo = max_ammo;
		}
	}
	else if (shoot_button_held) {
		internal_timer = internal_timer - elapsed;
	}
	else {
		internal_timer = 0.0f; // means that you can tap fire at infinite fire rate
	}
}

bool Gun::Shoot(glm::vec3 dir) {

	if (internal_timer <= 0.0f && cur_ammo > 0 && cur_state != reloading) {
		internal_timer = fire_rate_delay;
		cur_state = shooting;

		std::cout << "Shooting in dir: x ->" << dir.x << " y ->" << dir.y << "z ->" << dir.z << "\nAt vel: " << muzzle_velocity << std::endl;

		cur_ammo = glm::clamp(--cur_ammo, int16_t(0), max_ammo);

		return true;
	}

	return false;
}

bool Gun::Reload() {
	
	if (cur_ammo == max_ammo || cur_state == reloading) {
		return false;
	}

	reload_timer = reload_time;
	cur_state = reloading;
	return true;
}