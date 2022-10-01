#include "Player.hpp"

#include <glm/glm.hpp>
#include <iostream>

Gun::~Gun() {
}

Gun::Gun(Scene::Transform* player_transform, Scene::Transform* fire_point, int16_t max_ammo, float muzzle_velocity, float fire_rate_delay, float reload_time) {
	
	fire_point->parent = player_transform;

	this->reload_time = reload_time;
	this->max_ammo = max_ammo;
	this->cur_ammo = max_ammo;
	this->fire_rate_delay = fire_rate_delay;
	this->internal_timer = fire_rate_delay;
	this->muzzle_velocity = muzzle_velocity;
}

void Gun::UpdateTimer(float elapsed) {

	internal_timer = glm::clamp(internal_timer - elapsed, 0.0f, this->fire_rate_delay);
}

bool Gun::Shoot(glm::vec3 dir) {

	if (internal_timer <= 0.0f && cur_ammo > 0) {
		internal_timer = this->fire_rate_delay;

		std::cout << "Shooting in dir: x ->" << dir.x << " y ->" << dir.y << "z ->" << dir.z << "\nAt vel: " << this->muzzle_velocity << std::endl;

		cur_ammo = glm::clamp(--cur_ammo, int16_t(0), this->max_ammo);

		return true;
	}

	return false;
}