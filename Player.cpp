#include "Player.hpp"

Gun::~Gun() {
}

Gun::Gun(Scene::Transform* player_transform, Scene::Transform* fire_point, uint16_t max_ammo, float muzzle_velocity, float fire_rate_delay, float reload_time) {
	
	fire_point->parent = player_transform;

	this->reload_time = reload_time;
	this->max_ammo = max_ammo;
	this->cur_ammo = max_ammo;
	this->fire_rate_delay = fire_rate_delay;
	this->internal_timer = fire_rate_delay;
	this->muzzle_velocity = muzzle_velocity;
}